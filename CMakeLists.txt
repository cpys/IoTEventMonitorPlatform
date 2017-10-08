cmake_minimum_required(VERSION 3.8)
project(IoTEventMonitorPlatform)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 打开QT的MOC功能和UI文件处理功能
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

# 查找 QT的库
#set(CMAKE_PREFIX_PATH D:\\Qt\\5.9.1\\mingw53_32)
#set(Qt5Widgets_DIR D:\\Qt\\5.9.1\\mingw53_32\\lib\\cmake\\Qt5Widgets)
find_package(Qt5Widgets)

# 依赖库
#link_directories(D:\\Qt\\Tools\\mingw530_32\\i686-w64-mingw32\\lib)

# UI文件
#set(UI_FILES)
# .rc文件
#set(RC_FILES)
# .qrc文件
#set(QRC_FILES)
# 代码文件
set(SOURCE_FILES main.cpp CustomMainTabWidget.cpp CustomEventTabWidget.cpp CustomStateTabWidget.cpp CustomRunWidget.cpp CustomEventWidget.cpp CustomStateWidget.cpp CustomSubTabWidget.cpp CustomSubTabWidget.h CustomIpEdit.cpp CustomIpEdit.h ConstStyle.h)

add_executable(IoTEventMonitorPlatform ${UI_FILES} ${RC_FILES} ${QRC_FILES} ${SOURCE_FILES})

target_link_libraries(IoTEventMonitorPlatform Qt5::Widgets)