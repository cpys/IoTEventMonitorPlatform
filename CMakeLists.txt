cmake_minimum_required(VERSION 3.5)
project(IoTEventMonitorPlatform)

set(CMAKE_CXX_STANDARD 11)

# 打开QT的MOC功能功能
set(CMAKE_AUTOMOC ON)

# 查找 QT的库
#set(Qt5Widgets_DIR D:\\Qt\\5.9.2\\mingw53_32\\lib\\cmake\\Qt5Widgets)
set(Qt5Widgets_DIR /home/chenkuan/Qt/5.9.2/gcc_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets)
set(Qt5WebEngineWidgets_DIR /home/chenkuan/Qt/5.9.2/gcc_64/lib/cmake/Qt5WebEngineWidgets)
find_package(Qt5WebEngineWidgets)

# 依赖库
#link_directories(D:\\Qt\\Tools\\mingw530_32\\i686-w64-mingw32\\lib)

# 依赖头文件
include_directories(src/DFA)
include_directories(src/log)
include_directories(src/manager)
include_directories(src/memoryClient)
include_directories(src/netfilterClient)
include_directories(src/serialPortClient)
include_directories(src/tinyxml2)

# 代码文件
set(SRC_MAIN src/main.cpp)
aux_source_directory(src/DFA SRC_DFA)
aux_source_directory(src/GUI SRC_GUI)
aux_source_directory(src/log SRC_LOG)
aux_source_directory(src/manager SRC_MANAGER)
aux_source_directory(src/memoryClient SRC_MEMORY)
aux_source_directory(src/netfilterClient SRC_NETFILTER_CLIENT)
aux_source_directory(src/serialPortClient SRC_SERIAL_PORT_CLIENT)
aux_source_directory(src/tinyxml2 SRC_TINY_XML)

add_executable(IoTEventMonitorPlatform ${SRC_MAIN} ${SRC_DFA} ${SRC_GUI} ${SRC_LOG} ${SRC_MANAGER} ${SRC_MEMORY} ${SRC_NETFILTER_CLIENT} ${SRC_SERIAL_PORT_CLIENT} ${SRC_TINY_XML})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

# 链接QT库
target_link_libraries(IoTEventMonitorPlatform Qt5::Widgets)
target_link_libraries(IoTEventMonitorPlatform Qt5::WebEngineWidgets)

# 链接Z3
target_link_libraries(IoTEventMonitorPlatform z3)
