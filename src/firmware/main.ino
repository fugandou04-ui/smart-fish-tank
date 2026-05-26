/**
 * @file main.ino
 * @brief 智能鱼缸ESP32-C3主程序
 * @author ESP32 Firmware
 * @date 2026-05-21
 * 
 * 功能列表:
 * - WiFi连接与Web服务器
 * - 水泵/LED/舵机控制
 * - 温度传感器读取
 * - OLED显示
 * - 定时任务管理
 */

// ============================================
// 头文件
// ============================================
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FastLED.h>
#include <ESP32Servo.h>
#include <SSD1306Wire.h>
#include <time.h>

#include "config.h"
#include "webpage.h"

// ============================================
// 全局对象实例
// ============================================
Preferences g_preferences;
SystemState g_systemState;
ScheduleConfig g_scheduleConfig;

AsyncWebServer server(WEB_SERVER_PORT);

// 温度传感器
OneWire *oneWire;
DallasTemperature *ds18b20;

// LED
CRGB g_leds[LED_NUM];

// 舵机
Servo g_servo;

// OLED显示
SSD1306Wire *oled;

// ============================================
// 时间管理
// ============================================
time_t g_lastNtpSync = 0;
struct tm g_timeinfo;

// ============================================
// 函数声明
// ============================================
void initGPIO();
void initWiFi();
void initWebServer();
void initTemperature();
void initLED();
void initServo();
void initOLED();
void initTime();
void updateTemperature();
void updateOLED();
void checkSchedule();
void controlPump(bool on);
void controlLed(bool on, uint8_t brightness, uint32_t color);
void feedFish();
String getStatusJSON();
String getScheduleJSON();

// ============================================
// 配置管理函数
// ============================================
void loadConfig() {
    g_preferences.begin("fishtank", true);
    
    // 加载WiFi配置
    String ssid = g_preferences.getString(PrefKeys::wifiSsid, DEFAULT_WIFI_SSID);
    String pass = g_preferences.getString(PrefKeys::wifiPass, DEFAULT_WIFI_PASS);
    
    // 加载系统状态
    g_systemState.pumpOn = g_preferences.getBool(PrefKeys::pumpState, false);
    g_systemState.ledOn = g_preferences.getBool(PrefKeys::ledState, false);
    g_systemState.ledBrightness = g_preferences.getUShort(PrefKeys::ledBright, 128);
    g_systemState.ledColor = g_preferences.getULong(PrefKeys::ledColor, 0xFFFFFF);
    g_systemState.targetTemp = g_preferences.getFloat(PrefKeys::targetTemp, 25.0f);
    
    // 加载定时配置
    g_scheduleConfig.lightOnHour = g_preferences.getUChar(PrefKeys::lightOnHour, LIGHT_ON_HOUR);
    g_scheduleConfig.lightOnMinute = g_preferences.getUChar(PrefKeys::lightOnMin, LIGHT_ON_MINUTE);
    g_scheduleConfig.lightOffHour = g_preferences.getUChar(PrefKeys::lightOffHour, LIGHT_OFF_HOUR);
    g_scheduleConfig.lightOffMinute = g_preferences.getUChar(PrefKeys::lightOffMin, LIGHT_OFF_MINUTE);
    g_scheduleConfig.feedHour = g_preferences.getUChar(PrefKeys::feedHour, 9);
    g_scheduleConfig.feedMinute = g_preferences.getUChar(PrefKeys::feedMin, 0);
    g_scheduleConfig.autoLightEnabled = g_preferences.getBool(PrefKeys::autoLight, true);
    g_scheduleConfig.autoFeedEnabled = g_preferences.getBool(PrefKeys::autoFeed, true);
    
    g_preferences.end();
    
    Serial.printf("[Config] 配置加载完成 - SSID: %s\n", ssid.c_str());
}

void saveConfig() {
    g_preferences.begin("fishtank", false);
    
    g_preferences.putBool(PrefKeys::pumpState, g_systemState.pumpOn);
    g_preferences.putBool(PrefKeys::ledState, g_systemState.ledOn);
    g_preferences.putUShort(PrefKeys::ledBright, g_systemState.ledBrightness);
    g_preferences.putULong(PrefKeys::ledColor, g_systemState.ledColor);
    g_preferences.putFloat(PrefKeys::targetTemp, g_systemState.targetTemp);
    
    g_preferences.end();
    
    Serial.println("[Config] 配置保存完成");
}

void saveScheduleConfig() {
    g_preferences.begin("fishtank", false);
    
    g_preferences.putUChar(PrefKeys::lightOnHour, g_scheduleConfig.lightOnHour);
    g_preferences.putUChar(PrefKeys::lightOnMin, g_scheduleConfig.lightOnMinute);
    g_preferences.putUChar(PrefKeys::lightOffHour, g_scheduleConfig.lightOffHour);
    g_preferences.putUChar(PrefKeys::lightOffMin, g_scheduleConfig.lightOffMinute);
    g_preferences.putUChar(PrefKeys::feedHour, g_scheduleConfig.feedHour);
    g_preferences.putUChar(PrefKeys::feedMin, g_scheduleConfig.feedMinute);
    g_preferences.putBool(PrefKeys::autoLight, g_scheduleConfig.autoLightEnabled);
    g_preferences.putBool(PrefKeys::autoFeed, g_scheduleConfig.autoFeedEnabled);
    
    g_preferences.end();
    
    Serial.println("[Config] 定时配置保存完成");
}

void resetConfig() {
    g_preferences.begin("fishtank", false);
    g_preferences.clear();
    g_preferences.end();
    Serial.println("[Config] 配置已重置");
    ESP.restart();
}

// ============================================
// GPIO初始化
// ============================================
void initGPIO() {
    // 水泵继电器 - 低电平触发
    pinMode(PIN_PUMP_RELAY, OUTPUT);
    digitalWrite(PIN_PUMP_RELAY, HIGH);  // 默认关闭
    
    Serial.println("[GPIO] 初始化完成");
}

// ============================================
// WiFi初始化
// ============================================
void initWiFi() {
    loadConfig();
    
    String ssid = g_preferences.getString(PrefKeys::wifiSsid, "");
    String pass = g_preferences.getString(PrefKeys::wifiPass, "");
    
    // 如果没有保存的WiFi，进入AP模式
    if (ssid.length() == 0) {
        Serial.println("[WiFi] 无保存的WiFi，进入AP模式");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASS);
        Serial.printf("[WiFi] AP模式: %s\n", DEFAULT_WIFI_SSID);
    } else {
        Serial.printf("[WiFi] 连接到: %s\n", ssid.c_str());
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), pass.c_str());
        
        // 等待连接
        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < WIFI_RETRY_MAX) {
            delay(500);
            Serial.print(".");
            retry++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.printf("\n[WiFi] 连接成功! IP: %s\n", WiFi.localIP().toString().c_str());
        } else {
            Serial.println("\n[WiFi] 连接失败，进入AP模式");
            WiFi.mode(WIFI_AP);
            WiFi.softAP(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASS);
        }
    }
    
    // 保存当前SSID到配置
    g_preferences.begin("fishtank", false);
    g_preferences.putString(PrefKeys::wifiSsid, WiFi.SSID());
    g_preferences.end();
}

// ============================================
// Web服务器初始化
// ============================================
void initWebServer() {
    // 主页
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", INDEX_HTML);
    });
    
    // 获取状态
    server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "application/json", getStatusJSON().c_str());
    });
    
    // 水泵控制
    server.on("/api/pump", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("state")) {
            bool state = request->getParam("state")->value().toInt() == 1;
            controlPump(state);
            request->send_P(200, "application/json", API_STATUS_OK);
        } else {
            request->send_P(400, "application/json", API_STATUS_ERROR);
        }
    });
    
    // LED控制
    server.on("/api/led", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("state")) {
            bool state = request->getParam("state")->value().toInt() == 1;
            g_systemState.ledOn = state;
            controlLed(state, g_systemState.ledBrightness, g_systemState.ledColor);
            saveConfig();
            request->send_P(200, "application/json", API_STATUS_OK);
        } else if (request->hasParam("brightness")) {
            g_systemState.ledBrightness = request->getParam("brightness")->value().toInt();
            if (request->hasParam("color")) {
                g_systemState.ledColor = strtoul(request->getParam("color")->value().c_str(), NULL, 16);
            }
            controlLed(g_systemState.ledOn, g_systemState.ledBrightness, g_systemState.ledColor);
            saveConfig();
            request->send_P(200, "application/json", API_STATUS_OK);
        } else {
            request->send_P(400, "application/json", API_STATUS_ERROR);
        }
    });
    
    // 喂食控制
    server.on("/api/feed", HTTP_GET, [](AsyncWebServerRequest *request) {
        feedFish();
        request->send_P(200, "application/json", API_STATUS_OK);
    });
    
    // 定时配置
    server.on("/api/schedule", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("type")) {
            String type = request->getParam("type")->value();
            if (type == "light" && request->hasParam("auto")) {
                g_scheduleConfig.autoLightEnabled = request->getParam("auto")->value().toInt() == 1;
                saveScheduleConfig();
            } else if (type == "feed" && request->hasParam("auto")) {
                g_scheduleConfig.autoFeedEnabled = request->getParam("auto")->value().toInt() == 1;
                saveScheduleConfig();
            }
            request->send_P(200, "application/json", API_STATUS_OK);
        } else if (request->hasParam("lightOn")) {
            // 设置定时时间
            String lightOn = request->getParam("lightOn")->value();
            String lightOff = request->getParam("lightOff")->value();
            String feed = request->getParam("feed")->value();
            
            sscanf(lightOn.c_str(), "%d:%d", &g_scheduleConfig.lightOnHour, &g_scheduleConfig.lightOnMinute);
            sscanf(lightOff.c_str(), "%d:%d", &g_scheduleConfig.lightOffHour, &g_scheduleConfig.lightOffMinute);
            sscanf(feed.c_str(), "%d:%d", &g_scheduleConfig.feedHour, &g_scheduleConfig.feedMinute);
            
            saveScheduleConfig();
            request->send_P(200, "application/json", API_STATUS_OK);
        } else if (request->hasParam("get")) {
            request->send_P(200, "application/json", getScheduleJSON().c_str());
        } else {
            request->send_P(400, "application/json", API_STATUS_ERROR);
        }
    });
    
    // 系统配置
    server.on("/api/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("targetTemp")) {
            g_systemState.targetTemp = request->getParam("targetTemp")->value().toFloat();
            saveConfig();
            request->send_P(200, "application/json", API_STATUS_OK);
        } else if (request->hasParam("reset")) {
            resetConfig();
            request->send_P(200, "application/json", API_STATUS_OK);
        } else {
            request->send_P(400, "application/json", API_STATUS_ERROR);
        }
    });
    
    // WiFi配网
    server.on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("ssid") && request->hasParam("pass")) {
            String newSsid = request->getParam("ssid")->value();
            String newPass = request->getParam("pass")->value();
            
            g_preferences.begin("fishtank", false);
            g_preferences.putString(PrefKeys::wifiSsid, newSsid);
            g_preferences.putString(PrefKeys::wifiPass, newPass);
            g_preferences.end();
            
            request->send_P(200, "application/json", API_STATUS_OK);
            
            delay(1000);
            ESP.restart();
        } else {
            request->send_P(400, "application/json", API_STATUS_ERROR);
        }
    });
    
    server.begin();
    Serial.println("[WebServer] 服务器已启动");
}

// ============================================
// 温度传感器初始化
// ============================================
void initTemperature() {
    oneWire = new OneWire(PIN_ONEWIRE);
    ds18b20 = new DallasTemperature(oneWire);
    ds18b20->begin();
    ds18b20->setResolution(DS18B20_RESOLUTION);
    
    g_systemState.tempSensorOnline = ds18b20->getDeviceCount() > 0;
    Serial.printf("[Temp] 传感器初始化 %s\n", g_systemState.tempSensorOnline ? "成功" : "失败");
    
    // 立即读取一次温度
    updateTemperature();
}

// ============================================
// LED初始化
// ============================================
void initLED() {
    FastLED.addLeds<WS2812B, PIN_LED_DATA, GRB>(g_leds, LED_NUM);
    FastLED.setBrightness(0);
    FastLED.show();
    
    Serial.println("[LED] LED初始化完成");
}

// ============================================
// 舵机初始化
// ============================================
void initServo() {
    g_servo.attach(PIN_SERVO_PWM, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
    g_servo.write(0);  // 归位
    Serial.println("[Servo] 舵机初始化完成");
}

// ============================================
// OLED初始化
// ============================================
void initOLED() {
    oled = new SSD1306Wire(OLED_ADDRESS, PIN_OLED_SDA, PIN_OLED_SCL);
    oled->init();
    oled->flipScreenVertically();
    oled->setFont(ArialMT_Plain_10);
    
    Serial.println("[OLED] OLED初始化完成");
}

// ============================================
// NTP时间初始化
// ============================================
void initTime() {
    configTime(NTP_TIMEZONE * 3600, 0, NTP_SERVER);
    
    if (getLocalTime(&g_timeinfo)) {
        g_lastNtpSync = time(nullptr);
        Serial.printf("[Time] NTP同步成功: %s\n", asctime(&g_timeinfo));
    } else {
        Serial.println("[Time] NTP同步失败");
    }
    
    g_systemState.bootTime = time(nullptr);
}

// ============================================
// 温度读取
// ============================================
void updateTemperature() {
    ds18b20->requestTemperatures();
    float temp = ds18b20->getTempCByIndex(0);
    
    if (temp != DEVICE_DISCONNECTED_C && temp > -50 && temp < 100) {
        g_systemState.currentTemp = temp;
        g_systemState.tempSensorOnline = true;
        g_systemState.lastTempRead = time(nullptr);
    } else {
        g_systemState.tempSensorOnline = false;
    }
}

// ============================================
// OLED显示更新
// ============================================
void updateOLED() {
    static char line1[32], line2[32], line3[32], line4[32];
    
    // 格式化显示内容
    snprintf(line1, sizeof(line1), "Smart Fish Tank");
    snprintf(line2, sizeof(line2), "Temp: %.1f C", g_systemState.currentTemp);
    snprintf(line3, sizeof(line3), "Pump: %s  LED: %s", 
             g_systemState.pumpOn ? "ON" : "OFF",
             g_systemState.ledOn ? "ON" : "OFF");
    snprintf(line4, sizeof(line4), "IP: %s", WiFi.localIP().toString().c_str());
    
    oled->clear();
    oled->drawString(0, 0, line1);
    oled->drawString(0, 16, line2);
    oled->drawString(0, 32, line3);
    oled->drawString(0, 48, line4);
    oled->display();
}

// ============================================
// 定时任务检查
// ============================================
void checkSchedule() {
    if (!getLocalTime(&g_timeinfo)) return;
    
    static bool lastFeedDone = false;
    static bool lastLightCheck = false;
    
    int currentHour = g_timeinfo.tm_hour;
    int currentMin = g_timeinfo.tm_min;
    
    // 自动灯光控制
    if (g_scheduleConfig.autoLightEnabled) {
        if (currentHour == g_scheduleConfig.lightOnHour && 
            currentMin == g_scheduleConfig.lightOnMinute && !lastLightCheck) {
            controlLed(true, g_systemState.ledBrightness, g_systemState.ledColor);
            lastLightCheck = true;
            Serial.println("[Schedule] 自动开灯");
        } else if (currentHour == g_scheduleConfig.lightOffHour && 
                   currentMin == g_scheduleConfig.lightOffMinute && lastLightCheck) {
            controlLed(false, 0, 0);
            lastLightCheck = false;
            Serial.println("[Schedule] 自动关灯");
        }
    }
    
    // 自动喂食控制
    if (g_scheduleConfig.autoFeedEnabled) {
        if (currentHour == g_scheduleConfig.feedHour && 
            currentMin == g_scheduleConfig.feedMinute && !lastFeedDone) {
            feedFish();
            lastFeedDone = true;
            Serial.println("[Schedule] 自动喂食");
        } else if (currentMin != g_scheduleConfig.feedMinute) {
            lastFeedDone = false;
        }
    }
    
    // 每小时同步NTP
    if (currentMin == 0 && (time(nullptr) - g_lastNtpSync) > NTP_UPDATE_INTERVAL) {
        initTime();
    }
}

// ============================================
// 水泵控制
// ============================================
void controlPump(bool on) {
    g_systemState.pumpOn = on;
    digitalWrite(PIN_PUMP_RELAY, on ? LOW : HIGH);  // 低电平触发
    saveConfig();
    Serial.printf("[Pump] 水泵%s\n", on ? "开启" : "关闭");
}

// ============================================
// LED控制
// ============================================
void controlLed(bool on, uint8_t brightness, uint32_t color) {
    g_systemState.ledOn = on;
    g_systemState.ledBrightness = brightness;
    g_systemState.ledColor = color;
    
    if (on) {
        FastLED.setBrightness(brightness);
        for (int i = 0; i < LED_NUM; i++) {
            g_leds[i] = CRGB(
                (color >> 16) & 0xFF,
                (color >> 8) & 0xFF,
                color & 0xFF
            );
        }
    } else {
        FastLED.setBrightness(0);
        fill_solid(g_leds, LED_NUM, CRGB::Black);
    }
    FastLED.show();
    saveConfig();
    Serial.printf("[LED] 灯光%s 亮度:%d 颜色:0x%06X\n", on ? "开启" : "关闭", brightness, color);
}

// ============================================
// 喂食动作
// ============================================
void feedFish() {
    Serial.println("[Feed] 开始喂食...");
    
    // 舵机转动喂食
    g_servo.write(90);  // 转到喂食位置
    delay(FEED_DURATION);
    g_servo.write(0);   // 归位
    
    Serial.println("[Feed] 喂食完成");
}

// ============================================
// 获取状态JSON
// ============================================
String getStatusJSON() {
    static String json;
    json = "{";
    json += "\"success\":true,";
    json += "\"temperature\":" + String(g_systemState.currentTemp, 1) + ",";
    json += "\"pump\":" + String(g_systemState.pumpOn ? "true" : "false") + ",";
    json += "\"led\":" + String(g_systemState.ledOn ? "true" : "false") + ",";
    json += "\"brightness\":" + String(g_systemState.ledBrightness) + ",";
    json += "\"color\":\"" + String(g_systemState.ledColor, HEX) + "\",";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"ssid\":\"" + WiFi.SSID() + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
    json += "\"uptime\":" + String(time(nullptr) - g_systemState.bootTime);
    json += "}";
    return json;
}

// ============================================
// 获取定时配置JSON
// ============================================
String getScheduleJSON() {
    static String json;
    char lightOn[6], lightOff[6], feedTime[6];
    
    snprintf(lightOn, sizeof(lightOn), "%02d:%02d", g_scheduleConfig.lightOnHour, g_scheduleConfig.lightOnMinute);
    snprintf(lightOff, sizeof(lightOff), "%02d:%02d", g_scheduleConfig.lightOffHour, g_scheduleConfig.lightOffMinute);
    snprintf(feedTime, sizeof(feedTime), "%02d:%02d", g_scheduleConfig.feedHour, g_scheduleConfig.feedMinute);
    
    json = "{";
    json += "\"success\":true,";
    json += "\"lightOn\":\"" + String(lightOn) + "\",";
    json += "\"lightOff\":\"" + String(lightOff) + "\",";
    json += "\"feedTime\":\"" + String(feedTime) + "\",";
    json += "\"targetTemp\":" + String(g_systemState.targetTemp, 1) + ",";
    json += "\"autoLight\":" + String(g_scheduleConfig.autoLightEnabled ? "true" : "false") + ",";
    json += "\"autoFeed\":" + String(g_scheduleConfig.autoFeedEnabled ? "true" : "false");
    json += "}";
    return json;
}

// ============================================
// 初始化恢复状态
// ============================================
void restoreState() {
    // 恢复水泵状态
    if (g_systemState.pumpOn) {
        controlPump(true);
    }
    
    // 恢复LED状态
    if (g_systemState.ledOn) {
        controlLed(true, g_systemState.ledBrightness, g_systemState.ledColor);
    }
    
    Serial.println("[State] 状态恢复完成");
}

// ============================================
// 任务追踪变量
// ============================================
unsigned long g_lastTempRead = 0;
unsigned long g_lastOledUpdate = 0;

// ============================================
// Arduino主函数
// ============================================
void setup() {
    Serial.begin(115200);
    Serial.println("\n\n========================================");
    Serial.println("    智能鱼缸 ESP32-C3 固件 v1.0.0");
    Serial.println("========================================\n");
    
    // 初始化各模块
    initGPIO();
    initLED();
    initServo();
    initOLED();
    initTemperature();
    initWiFi();
    initWebServer();
    initTime();
    
    // 恢复保存的状态
    restoreState();
    
    Serial.println("\n[System] 初始化完成!");
    Serial.printf("[System] 访问地址: http://%s\n", WiFi.localIP().toString().c_str());
}

void loop() {
    unsigned long currentMillis = millis();
    
    // 定期读取温度
    if (currentMillis - g_lastTempRead >= TEMP_READ_INTERVAL) {
        updateTemperature();
        g_lastTempRead = currentMillis;
    }
    
    // 定期更新OLED
    if (currentMillis - g_lastOledUpdate >= OLED_UPDATE_INTERVAL) {
        updateOLED();
        g_lastOledUpdate = currentMillis;
    }
    
    // 检查定时任务
    checkSchedule();
    
    // 保持一定延迟
    delay(10);
}
