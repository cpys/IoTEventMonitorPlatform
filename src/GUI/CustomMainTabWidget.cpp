//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <iostream>
#include <QtWidgets/QTabBar>
#include "CustomMainTabWidget.h"
#include "conf.h"
#include <fstream>

CustomMainTabWidget::CustomMainTabWidget(QWidget *parent) : QTabWidget(parent) {
    eventTabWidget = new CustomEventTabWidget(this);
    stateTabWidget = new CustomStateTabWidget(this);
    runWidget = new CustomRunWidget(this);

    this->addTab(eventTabWidget, "事件模板定义");
    this->addTab(stateTabWidget, "事件状态机定义");
    this->addTab(runWidget, "运行展示");

    readConf();
    eventTabWidget->setConf(eventsConf);
    stateTabWidget->setConf(stateMachinesConf);
    runWidget->setConf(runConf);

    QObject::connect(eventTabWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
    QObject::connect(stateTabWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
    QObject::connect(runWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
}

void CustomMainTabWidget::saveConf() {
    eventTabWidget->saveConfToXML();
    stateTabWidget->saveConfToXML();
    runWidget->saveConfToXML();

//    // TODO Value function may be wrong
//    char *GUIStr = nullptr;
//    GUIConf.SaveFile(GUIStr);
//    this->writeConf(GUIStr);

    GUIConf.SaveFile(GUI_CONF_FILE);
}

void CustomMainTabWidget::paintEvent(QPaintEvent *event) {
    QTabWidget::paintEvent(event);
    // 只有当界面宽度变化时才进行标签的重绘
    if (currentWidth != width()) {
        currentWidth = width();
        changeTabStyle();
    }
}

void CustomMainTabWidget::changeTabStyle() {
    this->setStyleSheet((
                                "QTabWidget::pane {"
                                        "border: none;"
                                        "}"
                                        "QTabWidget::tab-bar {"
                                        "border: none;"
                                        "}"
                                        "QTabBar::tab {"
                                        "min-width: " + std::to_string(currentWidth / 3) + "px;"
                                        "height: " + std::to_string(MAIN_TAB_HEIGHT) + "px;"
                                        "}"
                        ).c_str());
}

void CustomMainTabWidget::readConf() {
    // 按配置文件、默认配置文件、默认配置内容顺序选择
    if(GUIConf.LoadFile(GUI_CONF_FILE) != XML_SUCCESS || !parseConf()) {
        if (GUIConf.LoadFile(GUI_CONF_DEFAULT_FILE) != XML_SUCCESS || !parseConf()) {
            GUIConf.Parse(GUI_CONF_TEMPLATE);
            parseConf();
        }
    }
}

bool CustomMainTabWidget::parseConf() {
    XMLElement *root = GUIConf.FirstChildElement();
    if (root == nullptr || strcmp(root->Attribute("projectName"), "IoTEventMonitorPlatform") != 0) {
        return false;
    }
    eventsConf = root->FirstChildElement("events");
    if (eventsConf == nullptr) {
        eventsConf = GUIConf.NewElement("events");
        root->InsertEndChild(eventsConf);
    }
    stateMachinesConf = root->FirstChildElement("stateMachines");
    if (stateMachinesConf == nullptr) {
        stateMachinesConf = GUIConf.NewElement("stateMachines");
        root->InsertEndChild(stateMachinesConf);
    }
    runConf = root->FirstChildElement("run");
    if (runConf == nullptr) {
        runConf = GUIConf.NewElement("run");
        root->InsertEndChild(runConf);
    }
    return true;
}

void CustomMainTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}
