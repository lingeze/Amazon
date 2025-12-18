# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(AmazonGame CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17) # 建议使用 C++17，现代且兼容性好
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(/utf-8)
# --- 1. 创建核心逻辑库 ---
# 查找 src 目录下的所有源文件 (.cpp)
file(GLOB CORE_SOURCES "src/*.cpp")

# 添加一个静态库，名为 core_logic，它只由源文件构成
add_library(core_logic STATIC ${CORE_SOURCES})

# 为 core_logic 库设置编译选项 (处理中文编码问题)
target_compile_options(core_logic PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# 指定 core_logic 库的头文件目录
# PUBLIC 表示：任何链接到 core_logic 的目标，都会自动获得这个头文件路径
target_include_directories(core_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


# --- 2. 创建 bot 可执行文件 ---
# 确认你的 bot 入口文件名是 bot.cpp
add_executable(my_bot bot/bot.cpp)

# 将 my_bot 链接到 core_logic 库
target_link_libraries(my_bot PRIVATE core_logic)

add_executable(test test/test.cpp)
target_link_libraries(test PRIVATE core_logic)
# --- 3. 创建 app (GUI) 可执行文件 ---
# 假设你的 GUI 由 Game.cpp 和 main_app.cpp 构成
# 把所有需要的 .cpp 文件都列在这里
# 请根据你的实际文件名修改！
add_executable(my_app 
    app/game.cpp 
)

# 同样将 my_app 链接到 core_logic 库
target_link_libraries(my_app PRIVATE core_logic)

# 如果你的 GUI 需要链接 SFML (根据我们之前的讨论)
# set(SFML_DIR ...)
# find_package(SFML ...)
# if(SFML_FOUND)
#     target_link_libraries(my_app PRIVATE sfml-graphics sfml-window sfml-system)
# endif()

# --- 4. 设置输出目录 ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

# --- 打印消息，方便调试 ---
message(STATUS "Configuring AmazonGame project...")
message(STATUS "Core logic library sources: ${CORE_SOURCES}")
message(STATUS "Bot executable will be 'my_bot'")
message(STATUS "App executable will be 'my_app'")

