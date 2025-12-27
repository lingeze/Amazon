# CMake 最低版本要求
# 【修改】添加 policy CMP0074 以消除警告并使用现代的 SFML_ROOT 行为
cmake_minimum_required(VERSION 3.10)
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# 项目名称
project(AmazonGame CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#add_compile_options(/utf-8)

# ===================================================================
# 设置 SFML 的根目录变量
# SFML_ROOT 是被 find_package 使用的官方变量，我们不再需要 SFML_DIR
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML") 

# 【关键修改 1/2】: 使用 SFML 3.0 的官方大写组件名
# -------------------------------------------------------------------
# 根据官方文档，组件名是 "System", "Window", "Graphics"。
find_package(SFML 3.0 COMPONENTS System Window Graphics REQUIRED)
# ===================================================================

# --- 1. 创建核心逻辑库 ---
file(GLOB CORE_SOURCES "src/*.cpp")
add_library(core_logic STATIC ${CORE_SOURCES})
target_include_directories(core_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(core_logic PUBLIC 
    $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/utf-8>
    $<$<NOT:$<COMPILE_LANG_AND_ID:CXX,MSVC>>:-finput-charset=UTF-8;-fexec-charset=UTF-8>
)
# 【关键修改 2/2】: 链接到 SFML 3.0 的官方命名空间目标
# -------------------------------------------------------------------
# 根据官方文档，链接目标是 "SFML::System", "SFML::Window", "SFML::Graphics"。
# SFML::Graphics 会自动处理对 SFML::Window 和 SFML::System 的依赖，
# 所以理论上我们只需要链接 SFML::Graphics 即可。为清晰起见，全部列出。
target_link_libraries(core_logic PUBLIC SFML::Graphics SFML::Window SFML::System)
# ===================================================================

# --- 2. 创建可执行文件 ---
add_executable(my_bot bot/bot.cpp)
target_link_libraries(my_bot PRIVATE core_logic)

add_executable(test test/test.cpp)
target_link_libraries(test PRIVATE core_logic)

add_executable(amazon_gui
    gui/gui.cpp
    gui/button.cpp
    gui/BoardDrawer.cpp 
    gui/PieceDrawer.cpp
    gui/Highlight.cpp
)
target_include_directories(amazon_gui PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui 
)
target_link_libraries(amazon_gui PRIVATE core_logic)

# --- 3. 设置输出目录和自动复制 DLL ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)
if (WIN32)
    file(GLOB DLL_FILES "${SFML_ROOT}/bin/*.dll")
    if(DLL_FILES)
        add_custom_command(TARGET amazon_gui POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL_FILES}
            $<TARGET_FILE_DIR:amazon_gui>
            COMMENT "Copying SFML DLLs to output directory"
        )
    endif()
endif()
set(SOURCE_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
if(EXISTS ${SOURCE_ASSETS_DIR})
    add_custom_command(
        TARGET amazon_gui POST_BUILD
        # ${CMAKE_COMMAND} -E copy_directory <源文件夹> <目标文件夹>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_ASSETS_DIR} ${CMAKE_BINARY_DIR}/assets
        COMMENT "Copying assets directory to build directory..."
    )
endif()
message(STATUS "Configuring AmazonGame project with official SFML 3.0 targets.")
