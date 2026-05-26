# 智能鱼缸 (Smart Fish Tank)

基于 ESP32-C3 的 DIY 桌面智能鱼缸控制系统。

## 功能特性

- 🌡️ 温度监测 (DS18B20)
- 💡 RGB LED 照明 (WS2812B)
- 🔄 自动水循环 (水泵控制)
- 🐟 定时喂食 (舵机)
- 📊 OLED 实时显示

## 项目结构

```
智能鱼缸/
├── docs/              # 设计文档（MD源文件）
│   ├── hardware/      # 硬件设计（PCB、电路）
│   ├── software/      # 软件设计
│   ├── structure/     # 结构设计
│   ├── management/     # 项目管理
│   └── process/        # 流程文档
├── src/firmware/      # ESP32-C3 固件源码
├── pcb/               # PCB 原理图文件
├── console/           # HTML 控制台 + 转换脚本
└── README.md          # 本文件
```

## 快速开始

### 硬件准备

| 器件 | 数量 | 说明 |
|------|------|------|
| ESP32-C3 Mini | 1 | 主控芯片 |
| OLED 0.66" | 1 | I2C 接口 |
| DS18B20 | 1 | 防水温度探头 |
| WS2812B LED | 12颗 | 灯带 |
| SG90 舵机 | 1 | 喂食控制 |
| 5V 水泵 | 1 | 带继电器 |
| 5V 继电器 | 2 | 水泵+预留 |

### GPIO 分配

| GPIO | 功能 |
|------|------|
| GPIO0 | OLED SCL (I2C) |
| GPIO1 | OLED SDA (I2C) |
| GPIO3 | 水泵继电器 |
| GPIO4 | 舵机 PWM |
| GPIO5 | WS2812B 数据 |
| GPIO7 | DS18B20 单总线 |

### 固件烧录

1. 安装 [Arduino IDE](https://www.arduino.cc/en/software)
2. 添加 ESP32 开发板：`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. 安装依赖库：AsyncTCP, ESP Async WebServer, FastLED, ESP32Servo, OneWire, DallasTemperature, SSD1306
4. 打开 `src/firmware/main.ino`
5. 选择开发板：`ESP32C3 Dev Module`
6. 编译上传

### Web 控制

1. 首次启动：连接热点 `FishTank_AP`，密码 `12345678`
2. 配置 WiFi 后访问分配到的 IP
3. 正常运行时直接访问设备 IP

## 预算

原型阶段总成本约 **92.2 元**

详见 `docs/hardware/元器件采购清单/元器件采购清单.md`

## 开发流程

本项目使用 Git + Worktree 管理工作：

```bash
# 创建新功能分支
git worktree add .worktrees/feature-xxx -b feature/xxx

# 开发完成后合并
git merge feature/xxx
git push origin main
```

## 文档说明

- 设计文档：`docs/` 目录
- HTML/DOCX 版本由 MD 源文件通过 `console/` 脚本转换生成
- 转换命令：`python console/md_to_html_apple.py`

## 仓库

https://github.com/fugandou04-ui/smart-fish-tank

## 许可证

MIT License