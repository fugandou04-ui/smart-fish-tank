# 智能鱼缸项目 (SFT-001)

## 项目概述
- 桌面智能鱼缸 15×15×15cm，基于 ESP32-C3 Mini
- 局域网 Web 控制，水泵/LED/舵机喂食/温度监测/OLED显示
- 预算 92.2 元

## 文件位置
- `E:\Mavis项目\智能鱼缸\`
- `console/index.html` - 文档中控台入口
- `docs/` - 13份设计文档（管理/硬件/软件/结构/工艺）
- `console/md_to_html_apple.py` - Markdown转Apple风格HTML脚本
- `console/batch_convert.py` - pandoc批量转换脚本

## 已完成工作
- 13份文档已创建，HTML/Word版本已完成
- 中控台和文档均为Apple极简白灰风格
- Word/PDF下载按钮已链接到实际文件

## 注意事项
- PDF需要安装LaTeX（MiKTeX）才能生成
- 浏览器缓存可能导致HTML不更新，需要重新生成文件
- pandoc安装后需刷新PATH环境变量