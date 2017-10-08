//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <iostream>
#include <QtWidgets/QTabBar>
#include "CustomMainTabWidget.h"

CustomMainTabWidget::CustomMainTabWidget(QWidget *parent) : QTabWidget(parent) {
    eventTabWidget = new CustomEventTabWidget(this);
    stateTabWidget = new CustomStateTabWidget(this);
    runWidget = new CustomRunWidget(this);

    this->addTab(eventTabWidget, "事件模板定义");
    this->addTab(stateTabWidget, "事件状态机定义");
    this->addTab(runWidget, "运行展示");

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

    //    this->setStyleSheet("QTabWidget::pane { "
//                                "border: none;"
////                                "border-top: 3px solid rgb(0, 0, 255);"
////                                "background: rgb(255, 255, 255);"
//                                "}"
//                                "QTabWidget::tab-bar {"
//                                "border: none;"
//                                "}"
//                                "QTabBar::tab {"
//                                "border: 1px solid transparent;"
//                                "color: white;"
//                                "background: transparent;"
//                                "height: 22px;"
//                                "min-width: 75px;"
//                                "}"
//                                "QTabBar::tab:hover {"
////                                "background: rgb(0, 0, 0);\n"
//                                "}"
//                                "QTabBar::tab:selected {"
////                                "background: rgb(255, 255, 255);\n"
//                                "}");
}

void CustomMainTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
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
