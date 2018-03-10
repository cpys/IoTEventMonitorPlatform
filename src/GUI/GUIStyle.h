//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_GUISTYLE_H
#define IOTEVENTMONITORPLATFORM_GUISTYLE_H

const int MAIN_WINDOW_WIDTH = 1000;
const int MAIN_WINDOW_HEIGHT = 600;

const int MAIN_TAB_HEIGHT = 40;

const int SUB_TAB_WIDTH = 100;
const int SUB_TAB_HEIGHT = 40;

const int EVENT_TAB_WIDTH = 100;
const int EVENT_TAB_HEIGHT = 40;

const int STATE_TAB_WIDTH = 100;
const int STATE_TAB_HEIGHT = 40;

const int CIRCLE_RADIUS = 100;

#define ENGLISH

#ifdef ENGLISH

#define WINDOW_TITLE "IoT Service System Monitor Platform"
#define EVENT_TEMPLATE_DEFINE "event define"
#define EVENT "event"
#define EVENT_HEAD "event head"
#define EVENT_BODY "event body"
#define EVENT_TAIL "event tail"
#define RUN_SHOW "run show"
#define EVENT_CHOOSE "event select"
#define EVENT_PREVIEW "event preview"
#define VM_IP "vm ip"
#define EXTERNAL_IP "externalip"
#define PSEUDO_TERMIAL_NAME "KVM pseudo termial"
#define SERIAL_NAME "serial name"
#define STATE_MACHINE_CHOOSE "state machine select"
#define RUN "run"
#define CLEAR "clear"

#else

#define WINDOW_TITLE "物联网服务系统监控平台"
#define EVENT_TEMPLATE_DEFINE "事件模板定义"
#define EVENT "事件"
#define EVENT_HEAD "事件匹配头部"
#define EVENT_BODY "事件非匹配主体"
#define EVENT_TAIL "事件匹配尾部"
#define RUN_SHOW "运行展示"
#define EVENT_CHOOSE "事件选择"
#define EVENT_PREVIEW "事件模板预览"
#define VM_IP "虚拟机ip"
#define EXTERNAL_IP "外部设备ip"
#define PSEUDO_TERMIAL_NAME "KVM伪终端名称"
#define SERIAL_NAME "宿主机串口名称"
#define STATE_MACHINE_CHOOSE "状态机选择"
#define RUN "启动"
#define CLEAR "清屏"

#endif

#endif //IOTEVENTMONITORPLATFORM_GUISTYLE_H
