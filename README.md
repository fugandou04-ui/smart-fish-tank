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
docs/           # 设计文档
src/firmware/   # ESP32 固件
pcb/            # PCB 原理图
console/        # Web 控制台
```

## 快速开始

1. 克隆仓库
2. 安装 PlatformIO
3. 编译固件：`pio run`
4. 上传：`pio run -t upload`

## GitHub

https://github.com/fugandou04-ui/smart-fish-tank