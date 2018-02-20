//
// Created by chenkuan on 2018/2/20.
//

#include "CustomTabWidget.h"
#include <GUIStyle.h>

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent) {
    eventManagerWidget = new CustomEventManagerWidget(this);
    runWidget = new CustomRunWidget(this);

    addTab(eventManagerWidget, EVENT_MANAGER_TITLE);
    addTab(runWidget, RUN_TITLE);

    // 设置间距风格
    setContentsMargins(0, 0, 0, 0);
}

void CustomTabWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    logger->debug("width %d --> %d", currentWidth, width());
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
