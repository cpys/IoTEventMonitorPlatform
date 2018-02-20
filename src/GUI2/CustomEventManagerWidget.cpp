//
// Created by chenkuan on 2018/2/20.
//

#include "CustomEventManagerWidget.h"
#include <CustomEventWidget.h>
#include <GUIStyle.h>

CustomEventManagerWidget::CustomEventManagerWidget(QWidget *parent) : QWidget(parent) {
    hBoxLayout = new QHBoxLayout(this);
    leftVBoxLayout = new QVBoxLayout();
    eventStackedWidget = new QStackedWidget(this);

    // 完成左右布局
    hBoxLayout->addLayout(leftVBoxLayout);
    hBoxLayout->addWidget(eventStackedWidget);

    eventListWidget = new QListWidget(this);
    addButton = new QPushButton(ADD_EVENT, this);
    deleteButton = new QPushButton(DELETE_EVENT, this);

    // 完成左侧的上下布局
    leftVBoxLayout->addWidget(eventListWidget);
    leftVBoxLayout->addWidget(addButton);
    leftVBoxLayout->addWidget(deleteButton);

    // 风格设置
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    leftVBoxLayout->setContentsMargins(0, 0, 0, 0);
    leftVBoxLayout->setSpacing(0);

    eventListWidget->setFixedWidth(EVENT_TAB_WIDTH);

    // 添加删除事件的响应
    QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(addEvent()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEvent()));

    // 使list与stackedWidget对应
    QObject::connect(eventListWidget, SIGNAL(currentRowChanged(int)), eventStackedWidget, SLOT(setCurrentIndex(int)));
}

void CustomEventManagerWidget::addEvent() {
    eventListWidget->addItem((EVENT + std::to_string(eventListWidget->count() + 1)).c_str());

    auto eventWidget = new CustomEventWidget(this);
    eventStackedWidget->addWidget(eventWidget);

    eventListWidget->setCurrentRow(eventListWidget->count() - 1);
}

void CustomEventManagerWidget::deleteEvent() {
    int currentRow = eventListWidget->currentRow();
    if (currentRow < 0) return;

    delete eventListWidget->takeItem(currentRow);

    QWidget *currentEventWidget = eventStackedWidget->widget(currentRow);
    eventStackedWidget->removeWidget(currentEventWidget);
    delete currentEventWidget;
}
