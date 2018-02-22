//
// Created by chenkuan on 2018/2/20.
//

#include "CustomTabWidget.h"
#include <GUIConf.h>

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent) {
    eventManagerWidget = new CustomEventManagerWidget(this);
    runWidget = new CustomRunWidget(this);

    addTab(eventManagerWidget, EVENT_MANAGER_TITLE);
    addTab(runWidget, RUN_TITLE);

    // 设置间距风格
    setContentsMargins(0, 0, 0, 0);

    // 将eventManagerWidget中的事件列表变更信号与runWidget中的信号槽连接起来
    QObject::connect(eventManagerWidget,
                     SIGNAL(insertEvent(int,
                                    const QString&, const QString&)),
                     runWidget,
                     SLOT(insertEvent(int,
                                  const QString&, const QString&)));
    QObject::connect(eventManagerWidget,
                     SIGNAL(removeEvent(int)),
                     runWidget,
                     SLOT(removeEvent(int)));
    QObject::connect(eventManagerWidget,
                     SIGNAL(modifyEvent(int,
                                    const QString&, const QString&)),
                     runWidget,
                     SLOT(modifyEvent(int,
                                  const QString&, const QString&)));

    // 载入配置
    loadConf();
}

void CustomTabWidget::loadConf() {
    // 按配置文件、默认配置文件、默认配置内容顺序选择
    if (GUIConf.LoadFile(GUI_CONF_FILE) != XML_SUCCESS || !parseConf()) {
        if (GUIConf.LoadFile(GUI_CONF_DEFAULT_FILE) != XML_SUCCESS || !parseConf()) {
            GUIConf.Parse(GUI_CONF_TEMPLATE);
            parseConf();
        }
    }
}

bool CustomTabWidget::parseConf() {
    XMLElement *root = GUIConf.FirstChildElement();
    if (root == nullptr || strcmp(root->Attribute("projectName"), PROJECT_NAME) != 0) {
        return false;
    }

    eventsConf = root->FirstChildElement("events");
    if (eventsConf == nullptr) {
        eventsConf = GUIConf.NewElement("events");
        root->InsertEndChild(eventsConf);
    }

    runConf = root->FirstChildElement("run");
    if (runConf == nullptr) {
        runConf = GUIConf.NewElement("run");
        root->InsertEndChild(runConf);
    }

    return true;
}

void CustomTabWidget::saveConf() {
    // TODO
}

void CustomTabWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    // 只有当界面宽度变化时才进行标签的重绘
    if (currentWidth != width()) {
        currentWidth = width();
        changeTabStyle();
    }
}

void CustomTabWidget::changeTabStyle() {
    this->setStyleSheet((
                                "QTabWidget::pane {"
                                        "border: none;"
                                        "}"
                                        "QTabWidget::tab-bar {"
                                        "border: none;"
                                        "}"
                                        "QTabBar::tab {"
                                        "min-width: " + std::to_string(currentWidth / 2) + "px;"
                                        "height: " + std::to_string(MAIN_TAB_HEIGHT) + "px;"
                                        "}"
                        ).c_str());
}