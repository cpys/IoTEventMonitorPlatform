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
    std::string confStr;

    std::ifstream inputConfFile(GUI_CONF_FILE, std::fstream::in);
    if (!inputConfFile) {
        readConfResult = false;
    }
    else {
        std::string line;
        while (getline(inputConfFile, line)) {
            confStr.append(line);
        }
        readConfResult = parseConf(confStr.c_str());
        inputConfFile.close();
    }

    // 如果文件不存在或者解析失败，则使用并写入默认配置
    if (!readConfResult) {
        parseConf(GUI_CONF_TEMPLATE);
        writeConf(GUI_CONF_TEMPLATE);
    }
}

bool CustomMainTabWidget::parseConf(const char *confStr) {
    XMLError xmlError = GUIConf.Parse(confStr);
    if (xmlError != XML_SUCCESS) {
        return false;
    }
    XMLElement *root = GUIConf.FirstChildElement();
    if (root == nullptr || root->Attribute("projectName") != "IoTEventMonitorPlatform") {
        return false;
    }

    eventConf = root->FirstChildElement("events");
    stateConf = root->FirstChildElement("stateMachines");
    runConf = root->FirstChildElement("run");
    return true;
}

void CustomMainTabWidget::writeConf(const std::string &confStr) {
    std::ofstream outputConfFile(GUI_CONF_FILE, std::ofstream::out);
    outputConfFile << confStr;
    outputConfFile.close();
}

void CustomMainTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}