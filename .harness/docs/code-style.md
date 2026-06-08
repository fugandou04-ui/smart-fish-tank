# 代码风格规范 — 智能鱼缸项目

> 路径：`E:\Mavis项目\智能鱼缸\.harness\docs\code-style.md`
> 维护人：project-manager + embedded-engineer
> 最近更新：2026-06-06

## 适用范围

- `src/firmware/` — Arduino/ESP32 固件（C++）
- `console/*.py` — Python 转换脚本
- `console/*.html` — HTML 控制台前端
- `pcb/` — JSON 配置文件

## 固件 C++ 规范

### 命名

- **变量/函数**：小驼峰 `camelCase`
  ```cpp
  int waterTempC;  // ✓
  int water_temp;  // ✗
  ```
- **常量/宏**：大写下划线 `UPPER_SNAKE_CASE`
  ```cpp
  #define LED_COUNT 12
  const int RELAY_PIN = 3;
  ```
- **类/结构体**：大驼峰 `PascalCase`
- **GPIO 引脚定义**：放 `config.h`，命名 `PIN_<功能>`
  ```cpp
  #define PIN_RELAY_PUMP 3
  ```

### 缩进

- 2 空格缩进（不用 Tab）
- Arduino IDE 默认 2 空格，PlatformIO 也可

### 注释

- 单行 `//`
- 函数头部用多行 `/* */` 描述
- **真值注释**：任何 `define` 旁必须写硬件真值
  ```cpp
  #define PIN_SDA 0   // OLED SDA (I2C0) — 硬件布线真值
  #define PIN_DS18B20 7  // 必须 GPIO7，避开 strapping 引脚
  ```

### 函数长度

- 单函数不超过 50 行
- 超过则拆分为 `setupXxx()` / `handleXxx()` / `updateXxx()`

### 字符串

- 嵌入式使用 `F()` 宏保存 RAM
  ```cpp
  Serial.println(F("Booting..."));
  ```

### 头文件

- 每个 `.ino` / `.h` 顶部有 `#ifndef` 保护（.h 文件）
- `#include` 顺序：内置库 → 第三方库 → 项目内

## Python 脚本规范

- PEP 8
- 函数定义下空 2 行
- 类型注解推荐（Python 3.9+）
- 中文注释 OK（项目内交流）

## HTML 控制台规范

- 单文件 HTML（embedded in `webpage.h`）
- 移动端优先（手机控制鱼缸）
- 暗色主题（减少 OLED 干扰）
- 不引入外部 CDN（嵌入式环境无外网）

## JSON 配置文件

- 2 空格缩进
- 文件末尾保留换行符
- key 用 snake_case

## 编辑器配置

推荐 `.editorconfig`：

```ini
root = true

[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
indent_style = space
indent_size = 2

[*.{ino,h,cpp}]
indent_size = 2

[*.py]
indent_size = 4
```

## 不允许

- ❌ 在 `.ino` 里写大量业务逻辑（拆函数）
- ❌ 死代码（不调用函数/宏，立即删除）
- ❌ 魔法数字（必须 `#define` 或 `const`）
- ❌ 跨文件全局变量（用 `extern` 或参数传递）
- ❌ printf-style 调试（生产代码用 Serial 替代）
