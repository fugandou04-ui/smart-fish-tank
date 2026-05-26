# 智能鱼缸 ESP32-C3 固件

基于ESP32-C3的智能鱼缸控制系统，支持WiFi连接、Web控制界面、水泵/灯光/喂食控制等功能。

## 功能特性

- **WiFi连接**: Station模式，支持自动重连
- **Web控制界面**: 响应式设计，实时状态显示
- **水泵控制**: 继电器驱动，GPIO3输出
- **灯光控制**: WS2812B全彩RGB，GPIO5数据
- **舵机喂食**: SG90 PWM控制，GPIO4
- **温度监测**: DS18B20单总线，GPIO7
- **OLED显示**: I2C接口(GPIO0/1)，实时状态显示
- **定时任务**: 自动开关灯、自动喂食

## 硬件连接

| 功能 | GPIO | 说明 |
|------|------|------|
| 水泵继电器 | GPIO3 | 低电平触发 |
| 舵机PWM | GPIO4 | SG90控制 |
| LED数据 | GPIO5 | WS2812B |
| 温度传感器 | GPIO7 | DS18B20 |
| OLED SCL | GPIO0 | I2C时钟 |
| OLED SDA | GPIO1 | I2C数据 |

## 文件结构

```
src/firmware/
├── main.ino      # 主程序
├── config.h      # 配置和引脚定义
├── webpage.h     # Web界面HTML
└── README.md     # 本文档
```

## 开发环境配置

### 1. 安装Arduino IDE

下载并安装 [Arduino IDE 2.x](https://www.arduino.cc/en/software)

### 2. 安装ESP32开发板

1. 打开 Arduino IDE
2. 进入 `文件` → `首选项`
3. 在"附加开发板管理器网址"中添加：
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. 进入 `工具` → `开发板` → `开发板管理器`
5. 搜索"ESP32"并安装

### 3. 安装依赖库

在 Arduino IDE 中进入 `工具` → `管理库`，安装以下库：

| 库名 | 版本 | 说明 |
|------|------|------|
| AsyncTCP | 1.1.4 | 异步TCP库 |
| ESP Async WebServer | 1.2.3 | 异步Web服务器 |
| FastLED | 3.9.2 | LED控制 |
| ESP32Servo | 0.13.0 | 舵机控制 |
| OneWire | 2.3.7 | 单总线协议 |
| DallasTemperature | 3.9.0 | DS18B20驱动 |
| SSD1306 | 1.8.5 | OLED驱动 |
| Preferences | - | 配置存储(内置) |

### 4. 选择开发板

1. `工具` → `开发板` → `ESP32 Arduino` → `ESP32C3 Dev Module`
2. 设置以下参数：
   - Upload Speed: 921600
   - CPU Frequency: 160MHz
   - Flash Size: 4MB
   - Partition Scheme: Default 4MB with spiffs

## 编译步骤

1. 打开 `main.ino`
2. 点击 `验证` (Ctrl+R) 编译项目
3. 确保无编译错误
4. 点击 `上传` (Ctrl+U) 烧录固件

## 首次配置

### 首次启动(AP模式)

1. 固件首次启动会创建热点
2. 连接WiFi: `FishTank_AP`
3. 密码: `12345678`
4. 浏览器访问 `192.168.4.1`
5. 在设置页面配置您的WiFi信息

### 正常运行

1. 固件连接家庭WiFi
2. 串口显示IP地址
3. 浏览器访问该IP进行控制

## Web控制界面

### 主界面功能

- **状态显示**: 温度、水泵状态、灯光状态、IP地址、运行时间
- **水泵控制**: 一键开关水泵
- **灯光控制**: 
  - 开关控制
  - 亮度滑块(0-255)
  - 9种预设颜色
  - 自动灯光开关
- **喂食控制**:
  - 手动喂食按钮
  - 自动喂食开关
- **定时设置**:
  - 开灯时间
  - 关灯时间
  - 喂食时间
  - 目标温度

### API接口

| 接口 | 方法 | 参数 | 说明 |
|------|------|------|------|
| `/` | GET | - | Web控制页面 |
| `/api/status` | GET | - | 获取系统状态 |
| `/api/pump` | GET | state=0/1 | 控制水泵 |
| `/api/led` | GET | state/brightness/color | 控制灯光 |
| `/api/feed` | GET | - | 触发喂食 |
| `/api/schedule` | GET | lightOn/lightOff/feed | 设置定时 |
| `/api/config` | GET | targetTemp | 设置目标温度 |
| `/api/wifi` | GET | ssid/pass | 配置WiFi |
| `/api/config` | GET | reset | 重置配置 |

## 测试步骤

### 基础功能测试

1. **串口通信测试**
   - 波特率: 115200
   - 检查启动日志
   - 确认各模块初始化成功

2. **WiFi连接测试**
   - 检查能否连接指定WiFi
   - 检查IP地址分配
   - 测试信号强度

3. **Web服务器测试**
   - 浏览器访问控制页面
   - 检查页面加载
   - 验证响应式设计

### 外设功能测试

4. **水泵测试**
   - 点击水泵开关
   - 听继电器吸合声
   - 确认水泵运行

5. **LED测试**
   - 开关灯光
   - 调整亮度
   - 选择不同颜色
   - 确认颜色正确

6. **舵机测试**
   - 点击喂食按钮
   - 观察舵机转动
   - 确认喂食时长

7. **温度传感器测试**
   - 检查温度显示
   - 对比实际水温
   - 验证数据更新

8. **OLED显示测试**
   - 确认屏幕正常显示
   - 检查信息完整性
   - 验证更新间隔

### 定时功能测试

9. **时间同步测试**
   - 检查NTP同步
   - 验证时间显示
   - 测试时区正确性

10. **定时任务测试**
    - 设置开灯时间
    - 设置关灯时间
    - 设置喂食时间
    - 观察自动执行

## 常见问题

### 编译错误

**错误**: `fatal error: xxx.h: No such file or directory`
- **解决**: 安装缺失的库

**错误**: `error: 'xxx' was not declared in this scope`
- **解决**: 检查库版本，尝试更新

### 上传失败

**错误**: `Failed to connect to ESP32`
- **解决**: 
  1. 按住BOOT按钮
  2. 点击上传
  3. 等待串口输出时松开BOOT

### WiFi连接失败

- 检查WiFi名称和密码
- 确认WiFi 2.4G频段(ESP32不支持5G)
- 检查路由器设置

### 温度显示异常

- 检查DS18B20接线
- 确认传感器地址
- 检查供电电压

## 扩展功能

### OTA升级

固件支持OTA无线升级，可通过Web界面或Arduino IDE远程更新。

### 自定义灯光效果

可扩展实现彩虹、呼吸灯等效果，修改`controlLed()`函数即可。

### 多传感器支持

DS18B20支持多个传感器级联，可在`initTemperature()`中配置。

## 技术支持

如有问题，请检查：
1. 串口日志输出
2. 硬件连接
3. 库版本兼容性

---

**版本**: v1.0.0  
**更新日期**: 2026-05-21  
**作者**: ESP32 Firmware Team
