#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Markdown转HTML - 苹果官网风格
"""

import os
import re

APPLE_STYLE = '''<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>TITLE_PLACEHOLDER</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        html { scroll-behavior: smooth; }
        body {
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 14px;
            line-height: 1.47059;
            letter-spacing: -0.022em;
            color: #1d1d1f;
            background: #ffffff;
            -webkit-font-smoothing: antialiased;
        }
        .hero {
            padding: 90px 0 60px;
            background: #f5f5f7;
            text-align: center;
        }
        .hero-content {
            max-width: 980px;
            margin: 0 auto;
            padding: 0 22px;
        }
        .eyebrow {
            font-size: 14px;
            font-weight: 600;
            color: #0071e3;
            margin-bottom: 8px;
        }
        .hero h1 {
            font-size: 48px;
            font-weight: 600;
            line-height: 1.0625;
            letter-spacing: -0.015em;
            margin-bottom: 12px;
        }
        .hero .subtitle {
            font-size: 21px;
            color: #86868b;
            max-width: 600px;
            margin: 0 auto 20px;
        }
        .meta-row {
            display: flex;
            justify-content: center;
            gap: 40px;
            padding-top: 30px;
            border-top: 1px solid #d2d2d7;
        }
        .meta-item {
            font-size: 14px;
            color: #86868b;
        }
        .meta-item strong { color: #1d1d1f; }
        nav {
            position: sticky;
            top: 0;
            background: rgba(255,255,255,0.8);
            backdrop-filter: saturate(180%) blur(20px);
            border-bottom: 1px solid rgba(0,0,0,0.1);
            z-index: 100;
        }
        .nav-content {
            max-width: 980px;
            margin: 0 auto;
            padding: 0 22px;
            display: flex;
            gap: 32px;
        }
        .nav-content a {
            padding: 16px 0;
            font-size: 12px;
            color: #1d1d1f;
            text-decoration: none;
            white-space: nowrap;
            opacity: 0.8;
            border-bottom: 2px solid transparent;
        }
        .nav-content a:hover { opacity: 1; }
        main {
            max-width: 980px;
            margin: 0 auto;
            padding: 60px 22px 100px;
        }
        h2 {
            font-size: 32px;
            font-weight: 600;
            margin: 60px 0 20px;
            padding-bottom: 12px;
            border-bottom: 1px solid #d2d2d7;
        }
        h3 { font-size: 21px; font-weight: 600; margin: 40px 0 14px; }
        h4 { font-size: 17px; font-weight: 600; margin: 28px 0 10px; }
        p { margin: 12px 0; }
        ul, ol { margin: 12px 0; padding-left: 24px; }
        li { margin: 8px 0; }
        table { width: 100%; border-collapse: collapse; margin: 20px 0; font-size: 14px; }
        th, td { border: 1px solid #d2d2d7; padding: 12px 14px; text-align: left; }
        th { background: #f5f5f7; font-weight: 600; }
        tr:nth-child(even) { background: #fbfbfd; }
        code {
            font-family: 'SF Mono', Consolas, monospace;
            font-size: 13px;
            background: #f5f5f7;
            padding: 2px 6px;
            border-radius: 4px;
        }
        pre {
            background: #1d1d1f;
            color: #f5f5f7;
            padding: 20px 24px;
            border-radius: 12px;
            overflow-x: auto;
            margin: 20px 0;
            font-size: 13px;
            line-height: 1.5;
        }
        pre code { background: none; padding: 0; color: inherit; }
        blockquote {
            border-left: 4px solid #0071e3;
            padding: 16px 20px;
            margin: 20px 0;
            background: #f5f5f7;
            border-radius: 0 12px 12px 0;
        }
        a { color: #0071e3; text-decoration: none; }
        a:hover { text-decoration: underline; }
        hr { border: none; border-top: 1px solid #d2d2d7; margin: 40px 0; }
        strong { font-weight: 600; }
        footer {
            padding: 30px 22px;
            border-top: 1px solid #d2d2d7;
            text-align: center;
            font-size: 12px;
            color: #86868b;
        }
        footer a { color: #0071e3; }
        @media (max-width: 768px) {
            .hero h1 { font-size: 32px; }
            .hero .subtitle { font-size: 17px; }
            .meta-row { flex-wrap: wrap; gap: 16px; }
            h2 { font-size: 24px; }
            h3 { font-size: 18px; }
        }
    </style>
</head>
<body>
    <section class="hero">
        <div class="hero-content">
            <p class="eyebrow">技术文档</p>
            <h1>TITLE_PLACEHOLDER</h1>
            <p class="subtitle">SUBTITLE_PLACEHOLDER</p>
            <div class="meta-row">
                <span class="meta-item">版本：<strong>VERSION_PLACEHOLDER</strong></span>
                <span class="meta-item">更新：<strong>DATE_PLACEHOLDER</strong></span>
                <span class="meta-item">SFT-001</span>
            </div>
        </div>
    </section>
    <nav>
        <div class="nav-content">
            <a href="#top">概述</a>
        </div>
    </nav>
    <main>
CONTENT_PLACEHOLDER
    </main>
    <footer>
        <p>SFT-001 智能鱼缸 · <a href="../../index.html">返回文档中心</a></p>
    </footer>
</body>
</html>'''

def md_to_html(md_text):
    lines = md_text.split('\n')
    html_lines = []
    in_table = False
    table_rows = []
    in_code = False
    code_content = []
    
    for line in lines:
        # 跳过分隔符
        if re.match(r'^[-*]{3,}$', line.strip()):
            continue
        
        # 代码块
        if line.strip().startswith('```'):
            if in_code:
                html_lines.append('<pre><code>' + '\n'.join(code_content) + '</code></pre>')
                code_content = []
                in_code = False
            else:
                in_code = True
            continue
        if in_code:
            code_content.append(line)
            continue
        
        # 标题
        if line.startswith('#### '):
            html_lines.append('<h4>' + line[5:] + '</h4>')
        elif line.startswith('### '):
            html_lines.append('<h3>' + line[4:] + '</h3>')
        elif line.startswith('## '):
            html_lines.append('<h2>' + line[3:] + '</h2>')
        elif line.startswith('# '):
            pass
        # 表格
        elif '|' in line:
            row = [c.strip() for c in line.split('|') if c.strip()]
            if row:
                table_rows.append(row)
                in_table = True
        else:
            if in_table and table_rows:
                html_lines.append('<table>')
                for i, row in enumerate(table_rows):
                    tag = 'th' if i == 0 else 'td'
                    html_lines.append('<tr>' + ''.join('<' + tag + '>' + c + '</' + tag + '>' for c in row) + '</tr>')
                html_lines.append('</table>')
                table_rows = []
                in_table = False
            
            line = line.strip()
            if not line:
                continue
            
            # 格式
            line = re.sub(r'\*\*(.+?)\*\*', r'<strong>\1</strong>', line)
            line = re.sub(r'`([^`]+)`', r'<code>\1</code>', line)
            
            if line.startswith('- '):
                html_lines.append('<li>' + line[2:] + '</li>')
            elif re.match(r'^\d+\. ', line):
                html_lines.append('<li>' + re.sub(r'^\d+\. ', '', line) + '</li>')
            else:
                html_lines.append('<p>' + line + '</p>')
    
    if in_table and table_rows:
        html_lines.append('<table>')
        for i, row in enumerate(table_rows):
            tag = 'th' if i == 0 else 'td'
            html_lines.append('<tr>' + ''.join('<' + tag + '>' + c + '</' + tag + '>' for c in row) + '</tr>')
        html_lines.append('</table>')
    
    return '\n            '.join(html_lines)

def get_info(md_content):
    title = "技术文档"
    subtitle = "智能鱼缸技术文档"
    version = "V1.0"
    date = "2026-05-22"
    
    for line in md_content.split('\n'):
        if line.startswith('# '):
            title = line[2:].strip()
            break
    
    for line in md_content.split('\n'):
        if '版本' in line:
            for part in line.split('|'):
                if '版本' in part:
                    v = part.replace('版本', '').strip()
                    if v: version = v
    for line in md_content.split('\n'):
        if '日期' in line:
            for part in line.split('|'):
                if '日期' in part:
                    d = part.replace('日期', '').strip()
                    if d: date = d[:10]
    
    return title, subtitle, version, date

def convert(md_path):
    with open(md_path, 'r', encoding='utf-8') as f:
        md = f.read()
    
    title, subtitle, version, date = get_info(md)
    content = md_to_html(md)
    
    html = APPLE_STYLE
    html = html.replace('TITLE_PLACEHOLDER', title)
    html = html.replace('SUBTITLE_PLACEHOLDER', subtitle)
    html = html.replace('VERSION_PLACEHOLDER', version)
    html = html.replace('DATE_PLACEHOLDER', date)
    html = html.replace('CONTENT_PLACEHOLDER', content)
    
    with open(md_path.replace('.md', '.html'), 'w', encoding='utf-8') as f:
        f.write(html)
    
    print('OK ' + os.path.basename(md_path))

def main():
    base = r"E:\Mavis项目\智能鱼缸\docs"
    files = []
    for root, dirs, fs in os.walk(base):
        for f in fs:
            if f.endswith('.md'):
                files.append(os.path.join(root, f))
    
    for f in files:
        convert(f)
    
    print('\nDone! ' + str(len(files)) + ' files')

if __name__ == '__main__':
    main()