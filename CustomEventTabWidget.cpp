//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include <string>
#include <iostream>
#include <QtWidgets/QMainWindow>
#include "CustomEventTabWidget.h"

CustomEventTabWidget::CustomEventTabWidget(QWidget *parent) : CustomSubTabWidget(parent) {
    auto eventWidget = new CustomEventWidget(this);
    this->insertTab(0, eventWidget, "1");
    this->setCurrentIndex(0);

    QObject::connect(eventWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
}

void CustomEventTabWidget::paintEvent(QPaintEvent *event) {
    CustomSubTabWidget::paintEvent(event);

    // 如果高度发生变化则重绘
    if (currentHeight == 0 || currentHeight != height()) {
        currentHeight = height();
        delete saveButton;
        delete deleteButton;
        saveButton = new QRect(0, currentHeight - EVENT_TAB_HEIGHT, EVENT_TAB_WIDTH, EVENT_TAB_HEIGHT);
        deleteButton = new QRect(0, currentHeight - 2 * EVENT_TAB_HEIGHT, EVENT_TAB_WIDTH, EVENT_TAB_HEIGHT);
    }

    QPainter painter(this);
    painter.drawRect(*saveButton);
    painter.drawRect(*deleteButton);
    painter.drawText(*saveButton, Qt::AlignCenter, "保存");
    painter.drawText(*deleteButton, Qt::AlignCenter, "删除");
}

void CustomEventTabWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    QPoint pos = event->pos();
    if (saveButton->contains(pos)) {
        // TODO save event
    }
    else if (deleteButton->contains(pos)) {
        int currentIndex = this->currentIndex();
        if (currentIndex != this->count() - 1) {
            emit sendStatusMessage("删除标签:" + this->tabText(currentIndex));
            // TODO delete confirm
            this->removeTab(currentIndex);
        }
    }
}

void CustomEventTabWidget::addCustomTab(int clickedTab) {
    if (clickedTab + 1 == this->count()) {
        if ((this->count() + 3) * EVENT_TAB_HEIGHT > currentHeight) {
            emit sendStatusMessage("无法添加更多事件");
        }
        else {
            auto eventWidget = new CustomEventWidget(this);
            this->insertTab(clickedTab, eventWidget, (std::to_string(clickedTab + 1)).c_str());
            QObject::connect(eventWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
        }
    }
}
