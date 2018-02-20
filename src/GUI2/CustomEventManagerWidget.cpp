//
// Created by chenkuan on 2018/2/20.
//

#include "CustomEventManagerWidget.h"
#include <GUIStyle.h>

CustomEventManagerWidget::CustomEventManagerWidget(QWidget *parent) : QWidget(parent) {
    hBoxLayout = new QHBoxLayout(this);
    leftVBoxLayout = new QVBoxLayout();
    eventStackedWidget = new QStackedWidget(this);

    // 完成左右布局
    hBoxLayout->addLayout(leftVBoxLayout, 1);
    hBoxLayout->addWidget(eventStackedWidget, 9);

    eventListWidget = new QListWidget(this);
    addButton = new QPushButton(ADD_EVENT, this);
    deleteButton = new QPushButton(DELETE_EVENT, this);

    // 完成左侧的上下布局
    leftVBoxLayout->addWidget(eventListWidget);
    leftVBoxLayout->addWidget(addButton);
    leftVBoxLayout->addWidget(deleteButton);
}