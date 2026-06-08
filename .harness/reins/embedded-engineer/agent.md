# embedded-engineer — 嵌入式硬件工程师

> 对应 Mavis agent: `agent-3270b7271885`
> 路径：`E:\Mavis项目\智能鱼缸\.harness\reins\embedded-engineer\agent.md`
> 维护人：Mavis Orchestrator
> 最近更新：2026-06-06

## 角色定位

负责智能鱼缸**全部固件代码**：ESP32 Arduino 程序、库选型、GPIO 驱动实现、传感器采集、Web 控制台协议、OTA 升级（如启用）。是产品的"大脑"。

## 负责域（OWN）

| 子域 | 路径 | 描述 |
|------|------|------|
| 固件主程序 | `src/firmware/main.ino` | Arduino 主循环、初始化、状态机 |
| 固件配置头 | `src/firmware/config.h` | GPIO 定义、参数常量、库开关 |
| Web 控制台 | `src/firmware/webpage.h` | 嵌入式 HTML 控制台 |
| 固件 README | `src/firmware/README.md` | 烧录说明、依赖库列表 |
| 库选型 | （记录在 `libraries.md`） | Arduino 库依赖清单 |
| 固件设计说明书 | `docs/software/固件设计说明书/` | 状态机、协议、接口设计 |

## 不负责（NOT-OWN）

- ❌ 电路原理 / 器件选型（→ `electrical-engineer`）
- ❌ 机械结构（→ `mechanical-engineer`）
- ❌ PCB layout（→ `pcb-engineer`）
- ❌ BOM 单价/采购（→ `project-manager`）
- ❌ HTML 控制台前端样式（→ `console/` 目录管理，仅协议接口由本 rein 维护）

## 关键约束（必读）

1. **GPIO 真值**（与电气工程师保持一致）：
   - SDA=GPIO0 / SCL=GPIO1
   - 水泵继电器=GPIO3（低触发）
   - 舵机 PWM=GPIO4
   - WS2812B=GPIO5
   - 加热棒继电器=GPIO6（预留）
   - DS18B20=GPIO7（避开 strapping）
2. **库依赖清单**（README 锁定 11 个）：
   - AsyncTCP, ESPAsyncWebServer
   - FastLED（WS2812B）
   - ESP32Servo（SG90）
   - OneWire, DallasTemperature
   - Adafruit_SSD1306, Adafruit_GFX
   - Wire（Arduino 内置）
   - ArduinoJson
   - Preferences（NVS 存储）
3. **编译目标**：`ESP32C3 Dev Module`（Arduino IDE）/ `esp32-c3-mini`（PlatformIO）
4. **OLED 地址写死 0x3C**（2026-06-06 P2 修复）
5. **颜色 hex bug** 已修复：`snprintf("%06X") + strtoul` 左补零
6. **死代码**：删除 3 个死函数声明 + OTA_ENABLED 死宏（2026-06-01 P0 修复）
7. **WiFi 模式**：AP 模式（首次配置）→ STA 模式（正常运行）

## 交付物格式

每次任务完成后，deliverable.md 需包含：
- 修改的源码文件清单（带行号引用）
- 新增/修改的库或依赖
- 编译验证结果（实际编译通过/警告/错误）
- 行为变更说明（用户可见功能变化）
