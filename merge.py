# controlled_merge.py
# This script abandons all failed "smart" logic and fully trusts the user's specified order.
# It is a dumb, but loyal and correct, executor.

import os
import re
import sys

# ==============================================================================
# === 请在这里编辑！请按照【从最基础到最上层】的顺序，填入您的头文件名 ===
# ==============================================================================
#
# 正确的顺序应该是：被依赖的（最基础的）在前，依赖别人的在后。
# 根据您的项目结构，这个顺序很可能是：
#
# 1. position.h  (坐标，最基础)
# 2. move.h      (移动，依赖坐标)
# 3. board.h     (棋盘，依赖移动和坐标)
# 4. player.h    (如果存在玩家基类)
# 5. Alplayer.h  (AI玩家，依赖棋盘、玩家等)
# ... 其他您自定义的类
#
HEADER_ORDER = [
    "position.h",
    "move.h",
    "board.h",
    "AIplayer.h",
    "Humanplayer.h",
    "filemanage.h"
]
# ==============================================================================

def get_pure_content(file_path):
    """读取文件，返回剥离了所有#include和#pragma once的纯代码内容。"""
    content = ""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            for line in f:
                stripped_line = line.strip()
                # 严格跳过所有 include 指令和 pragma once
                if stripped_line.startswith('#include') or stripped_line.startswith('#pragma once'):
                    continue
                content += line
    except FileNotFoundError:
        print(f"FATAL ERROR: The file '{file_path}' specified in your order does not exist.", file=sys.stderr)
        sys.exit(1)
    return content

def main():
    # --- 用户配置 ---
    SRC_DIR = "src"  # 您的源代码目录
    MAIN_CPP_FILE = "bot/bot.cpp" # 您的主函数文件
    OUTPUT_FILE = "bot_for_upload.cpp" # 输出文件名

    print("Starting merge process with user-defined order...")

    # 1. 收集所有系统头文件 <...>
    all_files = [os.path.join(SRC_DIR, f) for f in os.listdir(SRC_DIR)] + [MAIN_CPP_FILE]
    system_headers = set()
    for path in all_files:
        if not os.path.isfile(path): continue
        with open(path, 'r', encoding='utf-8') as f:
            for line in f:
                if re.match(r'#\s*include\s*<.*?>', line):
                    system_headers.add(line.strip())

    # --- 开始构建最终代码 ---
    
    # 序言
    final_code = "// Merged faithfully according to the user's explicit instructions.\n"
    final_code += "// My previous failures were unacceptable. This version is fully controlled.\n\n"
    
    # 第一部分：唯一的、去重后的系统头文件
    final_code += "// --- Part 1: System Headers ---\n"
    final_code += "\n".join(sorted(list(system_headers))) + "\n\n"

    # 第二部分：严格按照您定义的顺序，合并所有头文件
    final_code += "// --- Part 2: Class Definitions (in user-specified order) ---\n"
    print(f" -> Following user-defined header order: {' -> '.join(HEADER_ORDER)}")
    for header_file in HEADER_ORDER:
        final_code += f"// --- From: {header_file} ---\n"
        final_code += get_pure_content(os.path.join(SRC_DIR, header_file)) + "\n"

    # 第三部分：合并所有 .cpp 实现文件
    final_code += "// --- Part 3: Implementations ---\n"
    cpp_files = sorted([f for f in os.listdir(SRC_DIR) if f.endswith('.cpp')])
    for cpp_file in cpp_files:
        final_code += f"// --- From: {cpp_file} ---\n"
        final_code += get_pure_content(os.path.join(SRC_DIR, cpp_file)) + "\n"

    # 第四部分：合并主函数
    final_code += "// --- Part 4: Main Entry Point ---\n"
    final_code += f"// --- From: {os.path.basename(MAIN_CPP_FILE)} ---\n"
    final_code += get_pure_content(MAIN_CPP_FILE) + "\n"

    # 写入最终文件
    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        f.write(final_code)

    print(f"\nSUCCESS. The file '{OUTPUT_FILE}' has been generated exactly according to your specified order.")

if __name__ == "__main__":
    main()