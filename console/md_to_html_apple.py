#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Markdown转HTML转换器 - Apple极简风格
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
        :root {
            --bg-primary: #ffffff;
            --bg-secondary: #fbfbfd;
            --bg-tertiary: #f5f5f7;
            --text-primary: #1d1d1f;
            --text-secondary: #86868b;
            --text-tertiary: #6e6e73;
            --border-subtle: #e5e5ea;
            --border-light: #d2d2d7;
            --accent: #0071e3;
        }
        * { margin: 0; padding: 0; box-sizing: border-box; }
        html { scroll-behavior: smooth; }
        body {
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
            font-size: 14px;
            line-height: 1.8;
            color: var(--text-primary);
            background: var(--bg-primary);
            -webkit-font-smoothing: antialiased;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            padding: 0 40px 80px;
        }
        header {
            padding: 60px 0 50px;
            border-bottom: 1px solid var(--border-subtle);
            margin-bottom: 50px;
        }
        .doc-badge {
            display: inline-block;
            padding: 4px 12px;
            background: var(--bg-tertiary);
            border-radius: 16px;
            font-size: 11px;
            font-weight: 600;
            color: var(--text-tertiary);
            text-transform: uppercase;
            letter-spacing: 0.05em;
            margin-bottom: 16px;
        }
        h1 {
            font-size: 36px;
            font-weight: 600;
            letter-spacing: -0.02em;
            margin-bottom: 12px;
        }
        .doc-meta {
            display: flex;
            gap: 24px;
            font-size: 13px;
            color: var(--text-tertiary);
        }
        h2 {
            font-size: 24px;
            font-weight: 600;
            margin: 50px 0 20px;
            padding-bottom: 12px;
            border-bottom: 1px solid var(--border-subtle);
        }
        h3 { font-size: 18px; font-weight: 600; margin: 30px 0 12px; }
        h4 { font-size: 15px; font-weight: 600; margin: 24px 0 10px; }
        p { margin: 12px 0; }
        ul, ol { margin: 12px 0; padding-left: 24px; }
        li { margin: 8px 0; }
        table { width: 100%; border-collapse: collapse; margin: 20px 0; font-size: 13px; }
        th, td { border: 1px solid var(--border-subtle); padding: 12px 14px; text-align: left; }
        th { background: var(--bg-tertiary); font-weight: 600; }
        tr:nth-child(even) { background: var(--bg-secondary); }
        code { background: var(--bg-tertiary); padding: 2px 6px; border-radius: 4px; font-family: 'JetBrains Mono', Consolas, monospace; font-size: 13px; }
        pre { background: var(--bg-tertiary); padding: 16px 20px; border-radius: 8px; overflow-x: auto; margin: 16px 0; border: 1px solid var(--border-subtle); }
        pre code { background: none; padding: 0; }
        blockquote { border-left: 3px solid var(--accent); padding: 12px 20px; margin: 20px 0; background: var(--bg-secondary); color: var(--text-secondary); }
        a { color: var(--accent); text-decoration: none; }
        hr { border: none; border-top: 1px solid var(--border-subtle); margin: 40px 0; }
        strong { font-weight: 600; }
        .footer { margin-top: 60px; padding-top: 30px; border-top: 1px solid var(--border-subtle); text-align: center; font-size: 12px; color: var(--text-tertiary); }
        @media (max-width: 768px) { .container { padding: 0 20px 60px; } h1 { font-size: 28px; } h2 { font-size: 20px; } }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <div class="doc-badge">技术文档</div>
            <h1>TITLE_PLACEHOLDER</h1>
            <div class="doc-meta">
                <span>版本：VERSION_PLACEHOLDER</span>
                <span>更新：DATE_PLACEHOLDER</span>
            </div>
        </header>
        <main>
CONTENT_PLACEHOLDER
        </main>
        <footer class="footer">
            SFT-001 智能鱼缸技术文档 · 最后更新：DATE_PLACEHOLDER
        </footer>
    </div>
</body>
</html>'''

def md_to_html_content(md_text):
    """将Markdown内容转换为HTML"""
    lines = md_text.split('\n')
    html_lines = []
    in_table = False
    table_rows = []
    in_code_block = False
    code_content = []
    
    for line in lines:
        # 跳过纯分隔符行
        if re.match(r'^[-*]{3,}$', line.strip()) or re.match(r'^\|[-: ]+\|$', line.strip()):
            continue
        
        # 代码块
        if line.strip().startswith('```'):
            if in_code_block:
                html_lines.append('<pre><code>' + '\n'.join(code_content) + '</code></pre>')
                code_content = []
                in_code_block = False
            else:
                in_code_block = True
            continue
        
        if in_code_block:
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
            pass  # H1已在header中
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
                    cells = ''.join('<' + tag + '>' + cell + '</' + tag + '>' for cell in row)
                    html_lines.append('<tr>' + cells + '</tr>')
                html_lines.append('</table>')
                table_rows = []
                in_table = False
            
            line = line.strip()
            if not line:
                continue
            
            # 粗体 **text**
            line = re.sub(r'\*\*(.+?)\*\*', r'<strong>\1</strong>', line)
            # 行内代码 `code`
            line = re.sub(r'`([^`]+)`', r'<code>\1</code>', line)
            
            if line.startswith('- '):
                html_lines.append('<li>' + line[2:] + '</li>')
            elif re.match(r'^\d+\. ', line):
                num_line = re.sub(r'^\d+\. ', '', line)
                html_lines.append('<li>' + num_line + '</li>')
            else:
                html_lines.append('<p>' + line + '</p>')
    
    # 表格收尾
    if in_table and table_rows:
        html_lines.append('<table>')
        for i, row in enumerate(table_rows):
            tag = 'th' if i == 0 else 'td'
            cells = ''.join('<' + tag + '>' + cell + '</' + tag + '>' for cell in row)
            html_lines.append('<tr>' + cells + '</tr>')
        html_lines.append('</table>')
    
    return '\n            '.join(html_lines)

def extract_info(md_content):
    """从Markdown提取信息"""
    title = "技术文档"
    version = "V1.0"
    date = "2026-05-22"
    
    for line in md_content.split('\n'):
        if line.startswith('# '):
            title = line[2:].strip()
            break
    
    for line in md_content.split('\n'):
        if '版本' in line:
            parts = line.split('|')
            for part in parts:
                if '版本' in part:
                    version = part.replace('版本', '').replace('V', '').strip()
                    if not version.startswith('V'):
                        version = 'V' + version
    
    for line in md_content.split('\n'):
        if '日期' in line:
            parts = line.split('|')
            for part in parts:
                if '日期' in part:
                    date = part.replace('日期', '').strip()
                    if len(date) > 10:
                        date = date[:10]
    
    return title, version, date

def convert_file(md_path):
    """转换单个MD文件"""
    with open(md_path, 'r', encoding='utf-8') as f:
        md_content = f.read()
    
    title, version, date = extract_info(md_content)
    html_content = md_to_html_content(md_content)
    
    html = APPLE_STYLE
    html = html.replace('TITLE_PLACEHOLDER', title)
    html = html.replace('VERSION_PLACEHOLDER', version)
    html = html.replace('DATE_PLACEHOLDER', date)
    html = html.replace('CONTENT_PLACEHOLDER', html_content)
    
    html_path = md_path.replace('.md', '.html')
    with open(html_path, 'w', encoding='utf-8') as f:
        f.write(html)
    
    print('OK ' + os.path.basename(md_path))

def main():
    base_dir = r"E:\Mavis项目\智能鱼缸\docs"
    
    md_files = []
    for root, dirs, files in os.walk(base_dir):
        for file in files:
            if file.endswith('.md'):
                md_files.append(os.path.join(root, file))
    
    print('Found ' + str(len(md_files)) + ' MD files, converting...\n')
    
    for md_file in md_files:
        convert_file(md_file)
    
    print('\nDone! ' + str(len(md_files)) + ' HTML files generated')

if __name__ == '__main__':
    main()