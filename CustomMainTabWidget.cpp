//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include "CustomMainTabWidget.h"

CustomMainTabWidget::CustomMainTabWidget(QWidget *parent) : QTabWidget(parent) {
    eventTabWidget = new CustomEventTabWidget(parent);
    stateTabWidget = new CustomStateTabWidget(parent);
    runWidget = new CustomRunWidget(parent);

    this->addTab(eventTabWidget, "事件模板定义");
    this->addTab(stateTabWidget, "事件状态机定义");
    this->addTab(runWidget, "运行展示");

    this->setStyleSheet(
            "QTabWidget::pane {"
                    "border: none;"
            "}"
            "QTabWidget::tab-bar {"
                    "border: none;"
            "}"
            "QTabBar::tab {"
                    "min-width: 75px;"
                    "height: 30px;"
            "}"
    );

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

void CustomMainTabWidget::paintEvent(QPaintEvent *event) {
    QTabWidget::paintEvent(event);
}
