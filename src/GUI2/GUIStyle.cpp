//
// Created by chenkuan on 2018/2/20.
//

#include <GUIStyle.h>

const int MAIN_WINDOW_WIDTH = 1000;
const int MAIN_WINDOW_HEIGHT = 600;

const int MAIN_TAB_HEIGHT = 40;

const int EVENT_TAB_WIDTH = 100;
const int EVENT_TAB_HEIGHT = 40;

const char *ADD_EVENT = "+";
const char *DELETE_EVENT = "-";

#ifdef CHINESE
const char *MAIN_WINDOW_TITLE = "物联网服务系统监控平台";
const char *EVENT_MANAGER_TITLE = "事件模板定义";
const char *RUN_TITLE = "运行展示";
const char *EVENT_SELECT = "事件选择";
const char *VM_IP = "虚拟机ip";
const char *EXTERNAL_IP = "外部设备ip";
const char *PSEUDO_TERMINAL = "KVM伪终端名称";
const char *HOST_SERIAL_PORT = "宿主机串口名称";
const char *STATE_MACHINE_SELECT = "状态机文件选择";
const char *RUN = "启动";
const char *CLEAR = "清屏";
#endif

#ifdef ENGLISH
const char *MAIN_WINDOW_TITLE = "IoT service system monitor platform";
const char *EVENT_MANAGER_TITLE = "event template define";
const char *RUN_TITLE = "run & show";
#endif