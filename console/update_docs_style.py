#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
批量更新所有HTML文档为Apple极简风格
"""

import os
import re

# Apple Minimal Style CSS模板
APPLE_CSS = '''
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
            --accent-hover: #0077ed;
        }
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        html {
            scroll-behavior: smooth;
        }
        
        body {
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', 'PingFang SC', 'Microsoft YaHei', sans-serif;
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
        
        /* Header */
        header {
            padding: 60px 0 50px;
            border-bottom: 1px solid var(--border-subtle);
            margin-bottom: 50px;
        }
        
        .doc-badge {
            display: inline-flex;
            align-items: center;
            gap: 6px;
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
            color: var(--text-primary);
            margin-bottom: 12px;
            line-height: 1.2;
        }
        
        .doc-meta {
            display: flex;
            flex-wrap: wrap;
            gap: 24px;
            font-size: 13px;
            color: var(--text-tertiary);
        }
        
        .doc-meta span {
            display: flex;
            align-items: center;
            gap: 6px;
        }
        
        /* Sections */
        h2 {
            font-size: 24px;
            font-weight: 600;
            color: var(--text-primary);
            margin: 50px 0 20px;
            padding-bottom: 12px;
            border-bottom: 1px solid var(--border-subtle);
        }
        
        h3 {
            font-size: 18px;
            font-weight: 600;
            color: var(--text-primary);
            margin: 30px 0 12px;
        }
        
        h4 {
            font-size: 15px;
            font-weight: 600;
            color: var(--text-primary);
            margin: 24px 0 10px;
        }
        
        p {
            margin: 12px 0;
            color: var(--text-primary);
        }
        
        /* Lists */
        ul, ol {
            margin: 12px 0;
            padding-left: 24px;
        }
        
        li {
            margin: 8px 0;
            color: var(--text-primary);
        }
        
        /* Tables */
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            font-size: 13px;
        }
        
        th, td {
            border: 1px solid var(--border-subtle);
            padding: 12px 14px;
            text-align: left;
        }
        
        th {
            background: var(--bg-tertiary);
            font-weight: 600;
            color: var(--text-primary);
        }
        
        tr:nth-child(even) {
            background: var(--bg-secondary);
        }
        
        /* Code */
        code {
            background: var(--bg-tertiary);
            padding: 2px 6px;
            border-radius: 4px;
            font-family: 'JetBrains Mono', 'Fira Code', Consolas, monospace;
            font-size: 13px;
            color: var(--text-primary);
        }
        
        pre {
            background: var(--bg-tertiary);
            padding: 16px 20px;
            border-radius: 8px;
            overflow-x: auto;
            margin: 16px 0;
            border: 1px solid var(--border-subtle);
        }
        
        pre code {
            background: none;
            padding: 0;
        }
        
        /* Blockquote */
        blockquote {
            border-left: 3px solid var(--accent);
            padding: 12px 20px;
            margin: 20px 0;
            background: var(--bg-secondary);
            color: var(--text-secondary);
        }
        
        /* Links */
        a {
            color: var(--accent);
            text-decoration: none;
        }
        
        a:hover {
            text-decoration: underline;
        }
        
        /* HR */
        hr {
            border: none;
            border-top: 1px solid var(--border-subtle);
            margin: 40px 0;
        }
        
        /* Strong */
        strong {
            font-weight: 600;
        }
        
        /* Footer */
        .footer {
            margin-top: 60px;
            padding-top: 30px;
            border-top: 1px solid var(--border-subtle);
            text-align: center;
            font-size: 12px;
            color: var(--text-tertiary);
        }
        
        /* Responsive */
        @media (max-width: 768px) {
            .container {
                padding: 0 20px 60px;
            }
            
            h1 {
                font-size: 28px;
            }
            
            h2 {
                font-size: 20px;
            }
            
            table {
                font-size: 12px;
            }
            
            th, td {
                padding: 8px 10px;
            }
        }
        
        /* Scrollbar */
        ::-webkit-scrollbar {
            width: 8px;
        }
        
        ::-webkit-scrollbar-track {
            background: var(--bg-secondary);
        }
        
        ::-webkit-scrollbar-thumb {
            background: var(--border-light);
            border-radius: 4px;
        }
    </style>
</head>'''

APPLE_FOOTER = '''
    <div class="footer">
        SFT-001 智能鱼缸技术文档 · 最后更新：2026-05-22
    </div>
</body>'''

def update_html_file(filepath):
    """更新单个HTML文件为Apple极简风格"""
    
    with open(filepath, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 替换整个style标签
    content = re.sub(r'<style>.*?</style>', APPLE_CSS, content, flags=re.DOTALL)
    
    # 确保有doc-badge
    if 'class="doc-badge"' not in content:
        # 在h1前添加badge
        content = re.sub(r'(<h1[^>]*>)', '<div class="doc-badge">技术文档</div>\\1', content)
    
    # 添加footer
    if 'class="footer"' not in content:
        content = re.sub(r'</body>', APPLE_FOOTER, content)
    
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)
    
    print(f"更新: {os.path.basename(filepath)}")

def main():
    """批量更新所有HTML文档"""
    
    base_dir = r"E:\Mavis项目\智能鱼缸\docs"
    
    html_files = []
    for root, dirs, files in os.walk(base_dir):
        for file in files:
            if file.endswith('.html'):
                html_files.append(os.path.join(root, file))
    
    print(f"找到 {len(html_files)} 个HTML文件")
    
    for filepath in html_files:
        update_html_file(filepath)
    
    print(f"完成！已更新 {len(html_files)} 个文件")

if __name__ == '__main__':
    main()