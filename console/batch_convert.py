#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
批量生成 Word 和 PDF 文档
"""

import os
import subprocess

def convert_to_docx(md_path, output_path):
    """使用 pandoc 将 MD 转换为 DOCX"""
    cmd = [
        'pandoc',
        md_path,
        '-o', output_path,
        '--from', 'markdown',
        '--toc'
    ]
    try:
        subprocess.run(cmd, check=True, capture_output=True, shell=True)
        print('OK ' + os.path.basename(output_path))
    except Exception as e:
        print('FAIL ' + os.path.basename(output_path))

def main():
    base_dir = r"E:\Mavis项目\智能鱼缸\docs"
    
    md_files = []
    for root, dirs, files in os.walk(base_dir):
        for file in files:
            if file.endswith('.md'):
                md_files.append(os.path.join(root, file))
    
    print('=== Generating DOCX ===')
    for md_file in md_files:
        docx_path = md_path = md_file.replace('.md', '.docx')
        convert_to_docx(md_file, docx_path)
    
    print('\nDone!')

if __name__ == '__main__':
    main()