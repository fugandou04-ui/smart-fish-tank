#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Markdown转HTML转换器 - 智能鱼缸项目
"""

import os
import re

# 简单的HTML模板（用于将Markdown转换为带样式的HTML）
HTML_TEMPLATE = '''<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title}</title>
    <style>
        @page {{
            size: A4;
            margin: 20mm 15mm;
        }}
        
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        
        body {{
            font-family: "Microsoft YaHei", "微软雅黑", "SimSun", Arial, sans-serif;
            font-size: 12pt;
            line-height: 1.8;
            color: #333;
            background: white;
            max-width: 210mm;
            margin: 0 auto;
            padding: 20mm 15mm;
        }}
        
        h1 {{
            font-size: 22pt;
            text-align: center;
            margin: 30pt 0 20pt;
            color: #1a1a1a;
            border-bottom: 2px solid #667eea;
            padding-bottom: 10pt;
        }}
        
        h2 {{
            font-size: 16pt;
            margin: 25pt 0 12pt;
            color: #333;
            border-left: 4px solid #667eea;
            padding-left: 10pt;
        }}
        
        h3 {{
            font-size: 14pt;
            margin: 18pt 0 10pt;
            color: #444;
        }}
        
        p {{
            margin: 10pt 0;
            text-align: justify;
        }}
        
        table {{
            width: 100%;
            border-collapse: collapse;
            margin: 15pt 0;
            font-size: 10pt;
        }}
        
        th, td {{
            border: 1px solid #ddd;
            padding: 8pt 6pt;
            text-align: left;
        }}
        
        th {{
            background: #667eea;
            color: white;
            font-weight: bold;
        }}
        
        tr:nth-child(even) {{
            background: #f8f9fa;
        }}
        
        code {{
            background: #f4f4f4;
            padding: 2pt 4pt;
            border-radius: 3pt;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 10pt;
        }}
        
        pre {{
            background: #f6f8fa;
            padding: 12pt;
            border-radius: 6pt;
            overflow-x: auto;
            margin: 12pt 0;
            border: 1px solid #e1e4e8;
        }}
        
        pre code {{
            background: none;
            padding: 0;
        }}
        
        ul, ol {{
            margin: 10pt 0 10pt 20pt;
        }}
        
        li {{
            margin: 5pt 0;
        }}
        
        hr {{
            border: none;
            border-top: 1px solid #ddd;
            margin: 20pt 0;
        }}
        
        blockquote {{
            border-left: 4px solid #667eea;
            padding: 10pt 15pt;
            margin: 15pt 0;
            background: #f8f9fa;
            color: #555;
        }}
        
        strong {{
            color: #333;
        }}
        
        .header-info {{
            text-align: center;
            margin: 20pt 0;
            padding: 15pt;
            background: #f8f9fa;
            border-radius: 8pt;
        }}
        
        .footer {{
            margin-top: 30pt;
            padding-top: 15pt;
            border-top: 1px solid #ddd;
            text-align: center;
            color: #666;
            font-size: 10pt;
        }}
        
        @media print {{
            body {{
                padding: 0;
            }}
            
            h2 {{
                break-before: avoid;
            }}
            
            table {{
                break-inside: avoid;
            }}
        }}
    </style>
</head>
<body>
{content}
</body>
</html>'''

def markdown_to_html(md_content, title):
    """将Markdown内容转换为HTML"""
    
    content = md_content
    
    # 处理标题
    content = re.sub(r'^# (.+)$', r'<h1>\1</h1>', content, flags=re.MULTILINE)
    content = re.sub(r'^## (.+)$', r'<h2>\1</h2>', content, flags=re.MULTILINE)
    content = re.sub(r'^### (.+)$', r'<h3>\1</h3>', content, flags=re.MULTILINE)
    content = re.sub(r'^#### (.+)$', r'<h4>\1</h4>', content, flags=re.MULTILINE)
    
    # 处理表格
    lines = content.split('\n')
    in_table = False
    table_rows = []
    new_lines = []
    
    for line in lines:
        if '|' in line and re.match(r'^[\|\s\-:]+$', line):
            # 分隔行，跳过
            in_table = True
            continue
        elif '|' in line:
            row = line.strip()
            if row:
                cells = [c.strip() for c in row.split('|')]
                if cells and cells[0]:
                    table_rows.append(cells)
            continue
        else:
            if in_table and table_rows:
                # 生成表格
                if table_rows:
                    table_html = '<table>\n'
                    for i, row in enumerate(table_rows):
                        table_html += '<tr>'
                        for cell in row:
                            tag = 'th' if i == 0 else 'td'
                            table_html += f'<{tag}>{cell}</{tag}>'
                        table_html += '</tr>\n'
                    table_html += '</table>'
                    new_lines.append(table_html)
                table_rows = []
                in_table = False
            new_lines.append(line)
    
    content = '\n'.join(new_lines)
    
    # 处理列表
    content = re.sub(r'^\- (.+)$', r'<li>\1</li>', content, flags=re.MULTILINE)
    content = re.sub(r'^(\d+)\. (.+)$', r'<li>\2</li>', content, flags=re.MULTILINE)
    
    # 处理代码块
    content = re.sub(r'```[\w]*\n(.*?)```', r'<pre><code>\1</code></pre>', content, flags=re.DOTALL)
    content = re.sub(r'`([^`]+)`', r'<code>\1</code>', content)
    
    # 处理粗体和斜体
    content = re.sub(r'\*\*(.+?)\*\*', r'<strong>\1</strong>', content)
    content = re.sub(r'\*(.+?)\*', r'<em>\1</em>', content)
    
    # 处理水平线
    content = re.sub(r'^---+$', '<hr>', content, flags=re.MULTILINE)
    
    # 处理换行
    content = re.sub(r'\n\n+', '</p><p>', content)
    content = f'<p>{content}</p>'
    content = content.replace('<p><h', '<h')
    content = content.replace('</h2></p>', '</h2>')
    content = content.replace('</h3></p>', '</h3>')
    content = content.replace('</h4></p>', '</h4>')
    content = content.replace('<p><table>', '</p><table>')
    content = content.replace('</table></p>', '</table>')
    content = content.replace('<p><pre>', '</p><pre>')
    content = content.replace('</pre></p>', '</pre>')
    content = content.replace('<p><hr>', '</p><hr>')
    content = content.replace('<p></li>', '<li>')
    content = content.replace('</li></p>', '</li>')
    content = content.replace('</li><li>', '</li><li>')
    
    # 添加页脚
    footer = f'''
    <div class="footer">
        智能鱼缸项目技术文档 | SFT-001 V1.0 | 最后更新：2026-05-22
    </div>
    '''
    
    return HTML_TEMPLATE.format(title=title, content=content)

def convert_markdown_files():
    """转换所有Markdown文件到HTML"""
    
    base_dir = r"E:\Mavis项目\智能鱼缸\docs"
    
    for root, dirs, files in os.walk(base_dir):
        for file in files:
            if file.endswith('.md'):
                md_path = os.path.join(root, file)
                
                # 读取MD文件
                with open(md_path, 'r', encoding='utf-8') as f:
                    md_content = f.read()
                
                # 提取标题（第一个#开头的行）
                title = "智能鱼缸技术文档"
                for line in md_content.split('\n'):
                    if line.startswith('# '):
                        title = line[2:].strip()
                        break
                
                # 生成HTML文件
                html_content = markdown_to_html(md_content, title)
                html_path = md_path.replace('.md', '.html')
                
                with open(html_path, 'w', encoding='utf-8') as f:
                    f.write(html_content)
                
                print(f"转换: {md_path} -> {html_path}")

if __name__ == '__main__':
    convert_markdown_files()
    print("所有Markdown文件已转换为HTML格式")