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
}