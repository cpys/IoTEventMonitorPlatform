//
// Created by chenkuan on 2018/2/20.
//

#include <GUIConf.h>

// 配置文件位置
const char *GUI_CONF_FILE = "../conf/gui_conf.xml";
// 默认配置文件位置
const char *GUI_CONF_DEFAULT_FILE = "../conf/gui_conf_default.xml";
// 默认配置内容
const char *GUI_CONF_TEMPLATE = "<xml projectName=\"IoTEventMonitorPlatform\">\n</xml>";

const char *PROJECT_NAME = "IoTEventMonitorPlatform";

const int MAIN_WINDOW_WIDTH = 1000;
const int MAIN_WINDOW_HEIGHT = 600;

const int MAIN_TAB_HEIGHT = 40;

const int EVENT_TAB_WIDTH = 100;
const int EVENT_TAB_HEIGHT = 40;

const char *ADD_EVENT = "+";
const char *DELETE_EVENT = "-";

const char *DEFAULT_EVENT_MATCH_HEAD = "<xml>";
const char *DEFAULT_EVENT_MATCH_BODY = "<name>value</name>";
const char *DEFAULT_EVENT_MATCH_TAIL = "</xml>";

#ifdef CHINESE
const char *MAIN_WINDOW_TITLE = "物联网服务系统监控平台";

const char *EVENT_MANAGER_TITLE = "事件模板定义";
const char *EVENT = "事件";
const char *EVENT_MATCH_HEAD_DESC = "事件匹配头部";
const char *EVENT_MATCH_BODY_DESC = "事件非匹配主体";
const char *EVENT_MATCH_TAIL_DESC = "事件匹配尾部";

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