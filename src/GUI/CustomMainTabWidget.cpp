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

    // TODO Value function may be wrong
    char *GUIStr = nullptr;
    GUIConf.SaveFile(GUIStr);
    this->writeConf(GUIStr);
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
    if (!inputConfFile.is_open()) {
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

    // 如果文件不存在或者解析失败，则读取默认配置
    if (!readConfResult) {
        readConfResult = true;

        inputConfFile.open(GUI_CONF_DEFAULT_FILE, std::fstream::in);
        if (!inputConfFile.is_open()) {
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

        // 如果默认配置不存在或者读取后解析失败，则使用默认配置字符串
        if (!readConfResult) {
            parseConf(GUI_CONF_TEMPLATE);
        }
    }
}

bool CustomMainTabWidget::parseConf(const char *confStr) {
    XMLError xmlError = GUIConf.Parse(confStr);
    if (xmlError != XML_SUCCESS) {
        return false;
    }
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

void CustomMainTabWidget::writeConf(const char *confStr) {
    std::cout << "save file: " << confStr << std:: endl;
    std::ofstream outputConfFile(GUI_CONF_FILE, std::ofstream::out);
    outputConfFile << confStr;
    outputConfFile.close();
}

void CustomMainTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}
