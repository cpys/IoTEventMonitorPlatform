cmake_minimum_required(VERSION 3.8)
project(IoTEventMonitorPlatform)

set(CMAKE_CXX_STANDARD 11)

# 打开QT的MOC功能功能
set(CMAKE_AUTOMOC ON)

# 查找 QT的库
#set(CMAKE_PREFIX_PATH D:\\Qt\\5.9.1\\mingw53_32)
#set(Qt5Widgets_DIR D:\\Qt\\5.9.1\\mingw53_32\\lib\\cmake\\Qt5Widgets)
find_package(Qt5Widgets)

# 依赖库
#link_directories(D:\\Qt\\Tools\\mingw530_32\\i686-w64-mingw32\\lib)

# 依赖头文件
include_directories(src/tinyxml2)

# 代码文件
aux_source_directory(src SRC_MAIN)
aux_source_directory(src/GUI SRC_GUI)
aux_source_directory(src/tinyxml2 SRC_TINY_XML)
aux_source_directory(src/verification SRC_VERIFICATION)

add_executable(IoTEventMonitorPlatform ${SRC_MAIN} ${SRC_GUI} ${SRC_TINY_XML} ${SRC_VERIFICATION})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

# 链接QT库
target_link_libraries(IoTEventMonitorPlatform Qt5::Widgets)