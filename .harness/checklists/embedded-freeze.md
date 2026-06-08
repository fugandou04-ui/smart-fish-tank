# 嵌入式域 — 设计冻结 Checklist

> **基准文档**：`docs/software/固件设计说明书/固件设计说明书.md` v1.1（2026-06-02）
> **关联代码**：`src/firmware/main.ino`、`src/firmware/config.h`、`src/firmware/webpage.h`
> **负责人**：嵌入式硬件工程师 Agent（agent-3270b7271885）
> **目的**：所有固件代码、库依赖、烧录流程、行为在转入 PCB 打样 / 量产前必须通过本 checklist
> **使用方法**：每项必须为「✅ 已完成」或标注实际状态及阻塞原因

---

## 0. 元信息

| 项 | 内容 |
|------|------|
| Checklist 版本 | v1.0 |
| 编制日期 | 2026-06-06 |
| 关联项目 | 智能鱼缸（Smart Fish Tank）v2.0 设计冻结 |
| 适用范围 | main.ino / config.h / webpage.h / 库依赖 / 烧录 / OTA / Web API |
| 编译目标 | `ESP32C3 Dev Module`（Arduino IDE）/ `esp32-c3-mini`（PlatformIO） |
| 签字栏 | 嵌入式工程师 __________  日期 __________  verifier __________  日期 __________ |

---

## 一、文档与代码完整性

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 1.1 | 《固件设计说明书》v1.1 已发布到主分支 | ✅ 已具备 | 嵌入式工程师 | 2026-06-02 锁定 | |
| 1.2 | `src/firmware/main.ino` 编译无错误无警告 | ☐ 待编译 | 嵌入式工程师 | Arduino IDE 1.8.x / 2.x | |
| 1.3 | `src/firmware/config.h` 头文件保护完整（`#ifndef CONFIG_H`） | ✅ 已具备 | 嵌入式工程师 | L9 | |
| 1.4 | `src/firmware/webpage.h` 存在且无错误 | ☐ 待查 | 嵌入式工程师 | main.ino L31 include | |
| 1.5 | `src/firmware/README.md` 烧录说明完整 | ☐ 待查 | 嵌入式工程师 | 嵌入式 OWN 域 | |
| 1.6 | `libraries.md` 库选型清单与代码一致 | ☐ 待查 | 嵌入式工程师 | 嵌入式 OWN 域 | |
| 1.7 | 目录结构符合设计说明书 §1.3（src/main.cpp + devices/ + services/ + display/） | ⚠️ 部分偏离 | 嵌入式工程师 | 实际为 .ino 单文件，未拆模块，**待评估是否冻结前重构** | |

## 二、GPIO 真值同步

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 2.1 | `PIN_PUMP_RELAY = 3`（水泵继电器，低电平触发） | ✅ 已具备 | 嵌入式工程师 | config.h L16 | |
| 2.2 | `PIN_SERVO_PWM = 4`（舵机 SG90） | ✅ 已具备 | 嵌入式工程师 | config.h L17 | |
| 2.3 | `PIN_LED_DATA = 5`（WS2812B） | ✅ 已具备 | 嵌入式工程师 | config.h L18 | |
| 2.4 | `PIN_ONEWIRE = 7`（DS18B20，**避开 strapping**） | ✅ 已具备 | 嵌入式工程师 | config.h L19 | |
| 2.5 | `PIN_OLED_SDA = 0`（I2C SDA） | ✅ 已具备 | 嵌入式工程师 | config.h L21 | |
| 2.6 | `PIN_OLED_SCL = 1`（I2C SCL） | ✅ 已具备 | 嵌入式工程师 | config.h L20 | |
| 2.7 | `PIN_OLED_SDA/SCL` 与 PCB 文档一致 | ✅ 已具备 | 嵌入式工程师 | 注释明确引用 PCB | |
| 2.8 | GPIO 真值与电路文档 §四 引脚分配表 100% 对齐 | ✅ 已具备 | 嵌入式工程师 | 7 个 GPIO 全部对齐 | |
| 2.9 | **不引用 GPIO2**（v1.0 历史错误，v1.1 已修复） | ✅ 已具备 | 嵌入式工程师 | | |

## 三、库依赖（11 项锁定）

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 3.1 | `AsyncTCP`（ESP32 异步 TCP） | ✅ 已具备 | 嵌入式工程师 | main.ino L20 | |
| 3.2 | `ESPAsyncWebServer`（异步 Web 服务器） | ✅ 已具备 | 嵌入式工程师 | main.ino L21 | |
| 3.3 | `FastLED`（WS2812B LED 控制） | ✅ 已具备 | 嵌入式工程师 | main.ino L25 | |
| 3.4 | `ESP32Servo`（SG90 舵机 PWM） | ✅ 已具备 | 嵌入式工程师 | main.ino L26 | |
| 3.5 | `OneWire`（DS18B20 总线） | ✅ 已具备 | 嵌入式工程师 | main.ino L23 | |
| 3.6 | `DallasTemperature`（DS18B20 驱动） | ✅ 已具备 | 嵌入式工程师 | main.ino L24 | |
| 3.7 | `Adafruit_SSD1306`（OLED 驱动） | ⚠️ 检查 | 嵌入式工程师 | main.ino L27 实际为 `SSD1306Wire.h`（不同库），需统一 | |
| 3.8 | `Adafruit_GFX`（OLED 图形库） | ⚠️ 检查 | 嵌入式工程师 | main.ino L27 未显式 include | |
| 3.9 | `Wire`（I2C，Arduino 内置） | ✅ 已具备 | 嵌入式工程师 | Arduino 内置 | |
| 3.10 | `ArduinoJson`（JSON 解析） | ⚠️ 检查 | 嵌入式工程师 | main.ino 未显式 include（手写 JSON） | |
| 3.11 | `Preferences`（NVS 存储） | ✅ 已具备 | 嵌入式工程师 | main.ino L22 | |
| 3.12 | `platformio.ini` 依赖配置与代码一致 | ☐ 待查 | 嵌入式工程师 | 固件设计 §3.6 | |
| 3.13 | **库版本锁定**：ESPAsyncWebServer ≥1.2.0、AsyncTCP ≥1.1.4、OneWire ≥2.3.7、DallasTemperature ≥3.9.0、FastLED ≥3.6.0、ESP32Servo ≥0.12.0、Adafruit_GFX ≥1.11.0、Adafruit_SSD1306 ≥2.5.0、ArduinoJson ≥6.21.0 | ✅ 已规划 | 嵌入式工程师 | 固件设计 §3 | |

## 四、初始化与启动流程

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 4.1 | GPIO 初始化：PIN_PUMP_RELAY 设 OUTPUT，默认 HIGH（防止意外吸合） | ✅ 已具备 | 嵌入式工程师 | main.ino L156-160 | |
| 4.2 | I2C 总线初始化（OLED，0x3C） | ✅ 已具备 | 嵌入式工程师 | main.ino L374-381 | |
| 4.3 | OneWire 总线初始化（DS18B20，PIN_ONEWIRE） | ✅ 已具备 | 嵌入式工程师 | main.ino L338-349 | |
| 4.4 | 12 位分辨率设置（`DS18B20_RESOLUTION=12`） | ✅ 已具备 | 嵌入式工程师 | main.ino L342 + config.h L28 | |
| 4.5 | LED 初始化（FastLED `addLeds<WS2812B, PIN_LED_DATA, GRB>`） | ✅ 已具备 | 嵌入式工程师 | main.ino L354-360 | |
| 4.6 | LED 默认关闭（亮度 0） | ✅ 已具备 | 嵌入式工程师 | main.ino L356 | |
| 4.7 | 舵机初始化（attach + 归位 0°） | ✅ 已具备 | 嵌入式工程师 | main.ino L365-369 | |
| 4.8 | WiFi 模式：AP 模式（首次配置）→ STA 模式（正常运行） | ✅ 已具备 | 嵌入式工程师 | main.ino L166-204 | |
| 4.9 | NTP 同步 + 北京时区（+8） | ✅ 已具备 | 嵌入式工程师 | main.ino L386-397 + config.h L61-63 | |
| 4.10 | Flash 配置加载（Preferences 命名空间 "fishtank"） | ✅ 已具备 | 嵌入式工程师 | main.ino L84-111 | |
| 4.11 | 启动日志清晰（[Config] / [WiFi] / [OLED] 等模块前缀） | ✅ 已具备 | 嵌入式工程师 | 各 init 函数均有 Serial 输出 | |

## 五、核心功能实现

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 5.1 | 水泵控制：`controlPump(bool on)` 低电平触发 | ✅ 已具备 | 嵌入式工程师 | main.ino L485-490 | |
| 5.2 | LED 控制：`controlLed(on, brightness, color)` 颜色 hex 已左补零 | ✅ 已具备 | 嵌入式工程师 | main.ino L495-516, 颜色修复见 §7 | |
| 5.3 | 喂食动作：`feedFish()` 转 90° → delay → 归位 0° | ✅ 已具备 | 嵌入式工程师 | main.ino L521-530 + config.h L45 `FEED_DURATION=3000ms` | |
| 5.4 | 温度读取：`updateTemperature()` 容错（`DEVICE_DISCONNECTED_C`） | ✅ 已具备 | 嵌入式工程师 | main.ino L402-413 | |
| 5.5 | OLED 4 行显示：标题 / 温度 / 设备状态 / IP | ✅ 已具备 | 嵌入式工程师 | main.ino L418-435 | |
| 5.6 | 状态恢复：`restoreState()` 恢复水泵和 LED | ✅ 已具备 | 嵌入式工程师 | main.ino L587-599 | |
| 5.7 | 串口调试 115200 波特率 | ✅ 已具备 | 嵌入式工程师 | main.ino L611 | |
| 5.8 | 主循环非阻塞（`delay(10)` + `millis()` 调度） | ✅ 已具备 | 嵌入式工程师 | main.ino L633-653 | |

## 六、配置管理

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 6.1 | `loadConfig()` 从 Preferences 读取所有键 | ✅ 已具备 | 嵌入式工程师 | main.ino L84-111 | |
| 6.2 | `saveConfig()` 保存系统状态到 Preferences | ✅ 已具备 | 嵌入式工程师 | main.ino L113-125 | |
| 6.3 | `saveScheduleConfig()` 保存定时配置 | ✅ 已具备 | 嵌入式工程师 | main.ino L127-142 | |
| 6.4 | `resetConfig()` 清空 Preferences + ESP.restart() | ✅ 已具备 | 嵌入式工程师 | main.ino L144-150 | |
| 6.5 | PrefKeys 命名空间定义完整 | ✅ 已具备 | 嵌入式工程师 | config.h L96-120 | |
| 6.6 | WiFi 密码以 Base64 简单编码存储（**注意：非加密**） | ⚠️ 评估 | 嵌入式工程师 | 固件设计 §5.4，建议改用 NVS 加密分区 | |
| 6.7 | 写 Flash 频率合理（避免 1s 写一次） | ✅ 已具备 | 嵌入式工程师 | 仅在用户操作 / 定时任务变更时写 | |
| 6.8 | 启动时 CRC 校验（损坏用默认值） | ⚠️ 未实现 | 嵌入式工程师 | 固件设计 §5.3 规划未实现，**评估是否冻结前补** | |

## 七、已知 Bug 修复状态

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 7.1 | **颜色 hex bug 已修复**：`snprintf("%06X")` + `strtoul` 左补零 | ✅ 已修复 | 嵌入式工程师 | main.ino L242-251 (POST) + L548-550 (GET) | |
| 7.2 | **OLED 地址写死 0x3C**（2026-06-06 P2 修复） | ✅ 已修复 | 嵌入式工程师 | config.h L27 | |
| 7.3 | **死代码已删除**：3 个死函数声明 + `OTA_ENABLED` 死宏（2026-06-01 P0 修复） | ✅ 已修复 | 嵌入式工程师 | 嵌入式约束 #6 | |
| 7.4 | **DS18B20 引脚 GPIO2 → GPIO7**（v1.1 修复） | ✅ 已修复 | 嵌入式工程师 | 嵌入式约束 #1, config.h L19 | |
| 7.5 | `getStatusJSON` 中 color 序列化为 %06X（避免通道错位） | ✅ 已修复 | 嵌入式工程师 | main.ino L548-550 | |

## 八、Web API 协议

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 8.1 | `GET /` 返回 Web 界面（index.html） | ✅ 已具备 | 嵌入式工程师 | main.ino L211-213 | |
| 8.2 | `GET /api/status` 返回 JSON 状态 | ✅ 已具备 | 嵌入式工程师 | main.ino L216-218 | |
| 8.3 | `GET /api/pump?state=1/0` 控制水泵 | ✅ 已具备 | 嵌入式工程师 | main.ino L221-229 | |
| 8.4 | `GET /api/led?state=1/0` 控制 LED 开关 | ✅ 已具备 | 嵌入式工程师 | main.ino L232-238 | |
| 8.5 | `GET /api/led?brightness=&color=` 控制亮度+颜色 | ✅ 已具备 | 嵌入式工程师 | main.ino L239-258 | |
| 8.6 | `GET /api/feed` 触发喂食 | ✅ 已具备 | 嵌入式工程师 | main.ino L261-264 | |
| 8.7 | `GET /api/schedule?type=light&auto=1/0` 灯光自动 | ✅ 已具备 | 嵌入式工程师 | main.ino L267-272 | |
| 8.8 | `GET /api/schedule?type=feed&auto=1/0` 喂食自动 | ✅ 已具备 | 嵌入式工程师 | main.ino L273-276 | |
| 8.9 | `GET /api/schedule?lightOn=&lightOff=&feed=` 设置定时 | ✅ 已具备 | 嵌入式工程师 | main.ino L278-289 | |
| 8.10 | `GET /api/schedule?get=1` 获取定时 JSON | ✅ 已具备 | 嵌入式工程师 | main.ino L290-291 | |
| 8.11 | `GET /api/config?targetTemp=X` 设置目标温度 | ✅ 已具备 | 嵌入式工程师 | main.ino L298-302 | |
| 8.12 | `GET /api/config?reset=1` 恢复出厂 | ✅ 已具备 | 嵌入式工程师 | main.ino L303-305 | |
| 8.13 | `GET /api/wifi?ssid=&pass=` WiFi 配网 | ✅ 已具备 | 嵌入式工程师 | main.ino L312-329 | |
| 8.14 | **API 协议与电气 / 结构域不冲突** | ✅ 已具备 | 嵌入式工程师 | 仅协议层，跨域接口见 §十二 | |

## 九、JSON 数据结构

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 9.1 | `/api/status` 返回字段：success/temp/pump/led/brightness/color/ip/ssid/rssi/uptime | ✅ 已具备 | 嵌入式工程师 | main.ino L535-559 | |
| 9.2 | `/api/schedule` 返回字段：success/lightOn/lightOff/feedTime/targetTemp/autoLight/autoFeed | ✅ 已具备 | 嵌入式工程师 | main.ino L564-582 | |
| 9.3 | 错误响应统一 `API_STATUS_ERROR`（400） | ✅ 已具备 | 嵌入式工程师 | 各分支 | |
| 9.4 | 成功响应统一 `API_STATUS_OK`（200） | ✅ 已具备 | 嵌入式工程师 | 各分支 | |

## 十、定时任务调度

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 10.1 | 自动灯光：开灯时间 / 关灯时间触发 | ✅ 已具备 | 嵌入式工程师 | main.ino L450-462 | |
| 10.2 | 自动喂食：每日 9:00 触发（`FEED_HOUR=9`） | ✅ 已具备 | 嵌入式工程师 | main.ino L465-474 + config.h L47-48 | |
| 10.3 | 每小时 NTP 同步 | ✅ 已具备 | 嵌入式工程师 | main.ino L477-479 | |
| 10.4 | `lastFeedDone` / `lastLightCheck` 静态防重入 | ✅ 已具备 | 嵌入式工程师 | main.ino L443-444 | |
| 10.5 | 时间源依赖 NTP（断网后定时失效） | ⚠️ 评估 | 嵌入式工程师 | 离线场景需补 RTC，**冻结前评估** | |

## 十一、烧录与调试

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 11.1 | 烧录方式：USB-C 直连 ESP32-C3 Mini（板载 USB-Serial） | ✅ 已具备 | 嵌入式工程师 | 电路 §3.2.1 | |
| 11.2 | 烧录时按 BOOT → 复位 → 松开 BOOT 进入下载模式 | ✅ 已规划 | 嵌入式工程师 | 需用户手册说明 | |
| 11.3 | 4Pin 调试接口（TX/RX/3V3/GND）已预留 | ✅ 已具备 | 嵌入式工程师 | 电路 §3.9 | |
| 11.4 | 串口日志波特率 115200 | ✅ 已具备 | 嵌入式工程师 | main.ino L611 | |
| 11.5 | 启动日志包含版本号、IP、模块初始化状态 | ✅ 已具备 | 嵌入式工程师 | main.ino L612-630 | |
| 11.6 | OTA 升级接口预留 | ☐ 评估 | 嵌入式工程师 | 固件设计 §六 扩展功能，未实现 | |
| 11.7 | ESP32C3 Dev Module 烧录参数（Firmware Module, 4MB Flash, 80MHz） | ☐ 待确认 | 嵌入式工程师 | 实际模块型号 | |

## 十二、跨域接口一致性

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 12.1 | GPIO 真值与电路文档 100% 对齐 | ✅ 已具备 | 嵌入式 → 电气 | 3 处同步 | |
| 12.2 | LED_NUM=12 与机械结构 / 采购需求匹配 | ⚠️ 待核 | 嵌入式 → 电气 + 机械 | 缸盖 LED 槽长 150mm，30 LEDs/m → 应为 5 颗？ | |
| 12.3 | SERVO 脉宽 500~2500us 与 SG90 数据手册匹配 | ✅ 已具备 | 嵌入式工程师 | config.h L29-30 | |
| 12.4 | 喂食动作 FEED_DURATION=3000ms 满足机械 PT-09 拨食需求 | ☐ 待机械确认 | 嵌入式 → 机械 | 拨盘转动 90° 用时 3s 是否合理 | |
| 12.5 | `webpage.h` 颜色选择器 / 滑块 / 时间选择器与 JSON 协议匹配 | ☐ 待测 | 嵌入式工程师 | 端到端测试 | |
| 12.6 | 默认 SSID "FishTank_AP" / 密码 "12345678" 已写入用户手册 | ☐ 待 PM 整合 | 嵌入式 → PM | 用户级接口 | |

## 十三、待办 / 风险

| # | 检查项 | 当前状态 | 责任人 | 备注 | 完成日期 |
|---|--------|---------|--------|------|---------|
| 13.1 | 目录结构偏离设计说明书（实际为单 .ino，非模块化） | ⚠️ 评估 | 嵌入式工程师 | **冻结前决定是否重构** | |
| 13.2 | 库依赖不显式（Adafruit_SSD1306 / Adafruit_GFX / ArduinoJson 隐式） | ⚠️ 评估 | 嵌入式工程师 | **冻结前补 include 或在 README 说明** | |
| 13.3 | CRC 校验未实现 | ⚠️ 评估 | 嵌入式工程师 | **冻结前补或明确列为 v2.0+1** | |
| 13.4 | NTP 离线场景无 RTC 兜底 | ⚠️ 评估 | 嵌入式工程师 | **冻结前评估是否需要** | |
| 13.5 | WiFi 密码未加密（Base64 编码） | ⚠️ 评估 | 嵌入式工程师 | **建议改 NVS 加密或接受风险** | |
| 13.6 | OLED `SSD1306Wire.h` 与设计书 `Adafruit_SSD1306` 不一致 | ⚠️ 评估 | 嵌入式工程师 | **冻结前统一库名** | |
| 13.7 | 实际编译测试（在 ESP32-C3 Mini 真实硬件上跑通） | ☐ 待执行 | 嵌入式工程师 | **冻结前必经** | |
| 13.8 | 单元测试覆盖（web API 响应、JSON 序列化、温度采集） | ☐ 待执行 | 嵌入式工程师 | 建议补 | |

## 十四、签字确认

| 角色 | 姓名/Agent | 签字 | 日期 |
|------|------------|------|------|
| 嵌入式工程师 | embedded-engineer | __________ | __________ |
| 跨域评审 | electrical-engineer（`config.h` / GPIO 对齐） | __________ | __________ |
| 跨域评审 | mechanical-engineer（LED 数量 / 喂食时长确认） | __________ | __________ |
| 质量门禁 | verifier | __________ | __________ |

---

## 附录 A：未通过项处理流程

1. 未通过项立即在 board.md 标记 `blocked`
2. 24h 内由嵌入式工程师出整改方案
3. GPIO 变更必须 3 处同步：`config.h` + 电路文档 + MEMORY/README
4. 库版本变更必须在 `libraries.md` 记录
5. 任何破坏性变更必须升级并经 verifier 复核

## 附录 B：关联 checklist

- 结构域：`structure-freeze.md`
- 电气域：`electrical-freeze.md`
- PCB 域：`pcb-freeze.md`

---

**Checklist 结束 — 签字后方可进入 PCB 打样 / 量产采购阶段**
