//
// Created by chenkuan on 2018/2/20.
//

#include "CustomEventManagerWidget.h"
#include <CustomEventWidget.h>
#include <GUIStyle.h>
#include <qdebug.h>

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

    // 添加和删除事件的响应
    QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(addEvent()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEvent()));

    // 双击list的item响应，设置item为可编辑
    QObject::connect(eventListWidget,
                     SIGNAL(itemDoubleClicked(QListWidgetItem * )),
                     this,
                     SLOT(editListItem(QListWidgetItem * )));

    // list的item内容变化响应，item内容变化时emit信号
    QObject::connect(eventListWidget,
                     SIGNAL(itemChanged(QListWidgetItem * )),
                     this,
                     SLOT(afterEditEvent(QListWidgetItem * )));

    // 使list与stackedWidget对应
    QObject::connect(eventListWidget,
                     SIGNAL(currentRowChanged(int)),
                     eventStackedWidget,
                     SLOT(setCurrentIndex(int)));
}

void CustomEventManagerWidget::addEvent() {
    int pos = eventListWidget->count();
    QString eventName = EVENT + QString::number(pos + 1);
    eventListWidget->addItem(eventName);
    eventListWidget->setCurrentRow(pos);
    emit insertEvent(pos, eventName);

    auto eventWidget = new CustomEventWidget(this);
    eventStackedWidget->addWidget(eventWidget);
}

void CustomEventManagerWidget::deleteEvent() {
    int currentRow = eventListWidget->currentRow();
    if (currentRow < 0) return;

    delete eventListWidget->takeItem(currentRow);
    emit removeEvent(currentRow);

    QWidget *currentEventWidget = eventStackedWidget->widget(currentRow);
    eventStackedWidget->removeWidget(currentEventWidget);
    delete currentEventWidget;
}

void CustomEventManagerWidget::editListItem(QListWidgetItem *item) {
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}


void CustomEventManagerWidget::afterEditEvent(QListWidgetItem *item) {
    logger->debug("after modify, item->text:%s", item->text().toLocal8Bit().data());
    emit modifyEvent(eventListWidget->currentRow(), item->text());
}
