//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <iostream>
#include <QtWidgets/QTabBar>
#include "CustomMainTabWidget.h"
#include "conf.h"

CustomMainTabWidget::CustomMainTabWidget(QWidget *parent) : QTabWidget(parent) {
    eventTabWidget = new CustomEventTabWidget(this);
    stateTabWidget = new CustomStateTabWidget(this);
    runWidget = new CustomRunWidget(this);

    this->addTab(eventTabWidget, "事件模板定义");
    this->addTab(stateTabWidget, "事件状态机定义");
    this->addTab(runWidget, "运行展示");

    readConf();
    eventTabWidget->setConf(eventConf);
    stateTabWidget->setConf(stateConf);
    runWidget->setConf(runConf);

    QObject::connect(eventTabWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
    QObject::connect(stateTabWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
    QObject::connect(runWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
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
    bool readConfResult = true;

    std::fstream confFile(GUI_CONF_FILE, std::fstream::in | std::fstream::out);
    if (!confFile.is_open()) {
        readConfResult = false;
    }
    else {
        std::string confStr;
        confFile >> confStr;
        readConfResult = parseConf(confStr.c_str());
    }

    // 如果文件不存在或者解析失败，则写入默认配置
    if (!readConfResult) {
        parseConf(GUI_CONF_TEMPLATE);
        confFile << GUI_CONF_TEMPLATE;
    }
    confFile.close();
}

bool CustomMainTabWidget::parseConf(const char *confStr) {
    XMLError xmlError = GUIConf.Parse(confStr);
    if (xmlError != XML_SUCCESS) {
        return false;
    }
    XMLElement *root = GUIConf.FirstChildElement();
    if (root->Attribute("projectName") != "IoTEventMonitorPlatform") {
        return false;
    }

    // TODO 继续解析后续配置
}

void CustomMainTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}