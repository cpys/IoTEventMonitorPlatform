//
// Created by chenkuan on 2018/2/20.
//

#include "CustomEventManagerWidget.h"
#include <CustomEventWidget.h>
#include <GUIConf.h>
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
                     SLOT(afterEditEventName(QListWidgetItem * )));

    // 使list与stackedWidget对应
    QObject::connect(eventListWidget,
                     SIGNAL(currentRowChanged(int)),
                     eventStackedWidget,
                     SLOT(setCurrentIndex(int)));
}

void CustomEventManagerWidget::addEvent() {
    int row = eventListWidget->count();
    QString eventName = EVENT + QString::number(row + 1);
    eventListWidget->addItem(eventName);
    eventListWidget->setCurrentRow(row);

    auto eventWidget = new CustomEventWidget(this);
    eventStackedWidget->addWidget(eventWidget);
    eventStackedWidget->setCurrentIndex(row);
    // 为每个添加的eventWiget添加一个内容变化的消息响应
    QObject::connect(eventWidget, SIGNAL(eventContentChanged()),
                     this, SLOT(afterEditEventContent()));

    emit insertEvent(row, eventName, eventWidget->text());
}

void CustomEventManagerWidget::deleteEvent() {
    int currentRow = eventListWidget->currentRow();
    if (currentRow < 0) return;

    delete eventListWidget->takeItem(currentRow);

    QWidget *currentEventWidget = eventStackedWidget->widget(currentRow);
    eventStackedWidget->removeWidget(currentEventWidget);
    delete currentEventWidget;

    emit removeEvent(currentRow);
}

void CustomEventManagerWidget::editListItem(QListWidgetItem *item) {
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

void CustomEventManagerWidget::afterEditEventName(QListWidgetItem *item) {
    // 非当前行发生变化则忽略处理
    if (eventListWidget->currentItem() != item) return;

    logger->debug("after modify, item->text:%s", item->text().toLocal8Bit().data());
    int row = eventListWidget->currentRow();
    if (row < 0) return;
    auto eventWidget = dynamic_cast<CustomEventWidget *>(eventStackedWidget->widget(row));
    emit modifyEvent(row, item->text(), eventWidget->text());
}

void CustomEventManagerWidget::afterEditEventContent() {
    int row = eventListWidget->currentRow();
    if (row < 0) return;

    auto item = eventListWidget->item(row);
    auto eventWidget = dynamic_cast<CustomEventWidget *>(eventStackedWidget->widget(row));
    logger->debug("after modify, item->event->text:\n%s", eventWidget->text().toLocal8Bit().data());

    emit modifyEvent(row, item->text(), eventWidget->text());
}
