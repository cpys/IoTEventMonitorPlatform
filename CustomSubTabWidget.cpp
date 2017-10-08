//
// Created by yingzi on 2017/10/8.
//

#include "CustomSubTabWidget.h"

CustomSubTabWidget::CustomSubTabWidget(QWidget *parent) : QTabWidget(parent) {
    // 默认添加一个事件和一个加号
    emptyTab = new QWidget(this);
    this->addTab(emptyTab, "+");
    this->setTabPosition(West);

    QObject::connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(addCustomTab(int)));
    this->changeTabStyle();
}

void CustomSubTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}

void CustomSubTabWidget::changeTabStyle() {
    this->setStyleSheet((
                                "QTabWidget::pane {"
                                        "border: none;"
                                        "}"
                                        "QTabWidget::tab-bar {"
                                        "border: none;"
                                        "}"
                                        "QTabBar::tab {"
                                        "min-width: " + std::to_string(EVENT_TAB_WIDTH) + "px;"
                                        "height: " + std::to_string(EVENT_TAB_HEIGHT) + "px;"
                                        "}"
                        ).c_str());
}
