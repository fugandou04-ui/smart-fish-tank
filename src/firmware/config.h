/**
 * @file config.h
 * @brief 智能鱼缸配置头文件
 * @author ESP32 Firmware
 * @date 2026-05-21
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================
// GPIO引脚定义
// ============================================
#define PIN_PUMP_RELAY     3   // 水泵继电器控制 (低电平触发)
#define PIN_SERVO_PWM      4   // 舵机PWM控制 (SG90)
#define PIN_LED_DATA       5   // WS2812B数据引脚
#define PIN_ONEWIRE        7   // DS18B20单总线
#define PIN_OLED_SCL       0   // OLED I2C时钟
#define PIN_OLED_SDA       1   // OLED I2C数据

// ============================================
// 硬件配置
// ============================================
#define LED_NUM             12  // WS2812B LED数量
#define OLED_ADDRESS        0x3C  // SSD1306 I2C地址
#define DS18B20_RESOLUTION  12  // 温度分辨率 (9-12位)
#define SERVO_MIN_PULSE     500  // 舵机最小脉宽 (us)
#define SERVO_MAX_PULSE     2500 // 舵机最大脉宽 (us)
#define SERVO_MIN_ANGLE     0   // 舵机最小角度
#define SERVO_MAX_ANGLE     180 // 舵机最大角度

// ============================================
// WiFi配置默认值
// ============================================
#define DEFAULT_WIFI_SSID   "FishTank_AP"
#define DEFAULT_WIFI_PASS   "12345678"
#define WIFI_RETRY_MAX      10  // 最大重连次数
#define WIFI_RETRY_DELAY    500 // 重连延迟 (ms)

// ============================================
// 定时任务配置
// ============================================
#define FEED_DURATION       3000   // 喂食时长 (ms)
#define LIGHT_ON_HOUR       8      // 开灯小时 (24小时制)
#define LIGHT_ON_MINUTE     0      // 开灯分钟
#define LIGHT_OFF_HOUR      20     // 关灯小时
#define LIGHT_OFF_MINUTE    0      // 关灯分钟
#define TEMP_READ_INTERVAL  30000  // 温度读取间隔 (ms)
#define OLED_UPDATE_INTERVAL 1000 // OLED刷新间隔 (ms)

// ============================================
// Web服务器配置
// ============================================
#define WEB_SERVER_PORT     80
#define OTA_ENABLED          true

// ============================================
// NTP时间服务器
// ============================================
#define NTP_SERVER          "pool.ntp.org"
#define NTP_TIMEZONE        8      // 北京时区 +8
#define NTP_UPDATE_INTERVAL 3600  // NTP同步间隔 (秒)

// ============================================
// 状态结构体
// ============================================
struct SystemState {
    bool pumpOn;              // 水泵状态
    bool ledOn;               // LED状态
    bool autoFeedEnabled;     // 自动喂食使能
    uint8_t ledBrightness;    // LED亮度 (0-255)
    uint32_t ledColor;        // LED颜色 (RGB)
    float targetTemp;         // 目标温度
    float currentTemp;        // 当前温度
    bool tempSensorOnline;    // 温度传感器在线状态
    time_t lastTempRead;      // 上次温度读取时间
    time_t lastOledUpdate;    // 上次OLED更新时间
    time_t bootTime;          // 启动时间
};

struct ScheduleConfig {
    uint8_t lightOnHour;      // 开灯时间-时
    uint8_t lightOnMinute;    // 开灯时间-分
    uint8_t lightOffHour;     // 关灯时间-时
    uint8_t lightOffMinute;   // 关灯时间-分
    uint8_t feedHour;         // 喂食时间-时
    uint8_t feedMinute;       // 喂食时间-分
    bool autoLightEnabled;    // 自动灯光使能
    bool autoFeedEnabled;     // 自动喂食使能
};

// ============================================
// Preferences键名
// ============================================
namespace PrefKeys {
    // WiFi配置
    const char* wifiSsid = "wifi_ssid";
    const char* wifiPass = "wifi_pass";
    
    // 系统配置
    const char* pumpState = "pump_state";
    const char* ledState = "led_state";
    const char* ledBright = "led_bright";
    const char* ledColor = "led_color";
    const char* targetTemp = "target_temp";
    
    // 定时配置
    const char* lightOnHour = "light_on_hour";
    const char* lightOnMin = "light_on_min";
    const char* lightOffHour = "light_off_hour";
    const char* lightOffMin = "light_off_min";
    const char* feedHour = "feed_hour";
    const char* feedMin = "feed_min";
    const char* autoLight = "auto_light";
    const char* autoFeed = "auto_feed";
    
    // 校准配置
    const char* tempOffset = "temp_offset";
}

// ============================================
// 全局变量声明
// ============================================
extern SystemState g_systemState;
extern ScheduleConfig g_scheduleConfig;
extern Preferences g_preferences;

// ============================================
// 函数声明
// ============================================
void loadConfig();
void saveConfig();
void savePumpState(bool state);
void saveLedState(bool state);
void saveLedConfig(uint8_t brightness, uint32_t color);
void saveScheduleConfig();
void resetConfig();

#endif // CONFIG_H
