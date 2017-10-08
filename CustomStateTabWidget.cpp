//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include "CustomStateTabWidget.h"

CustomStateTabWidget::CustomStateTabWidget(QWidget *parent) : CustomSubTabWidget(parent) {
    auto stateWidget = new CustomStateWidget(this);
    this->insertTab(0, stateWidget, "1");
    this->setCurrentIndex(0);

    QObject::connect(stateWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
}

void CustomStateTabWidget::paintEvent(QPaintEvent *event) {
    QTabWidget::paintEvent(event);
    
    // 如果高度发生变化则重绘
    if (currentHeight == 0 || currentHeight != height()) {
        currentHeight = height();
        delete saveButton;
        delete viewXMLButton;
        delete deleteButton;
        saveButton = new QRect(0, currentHeight - STATE_TAB_HEIGHT, STATE_TAB_WIDTH, STATE_TAB_HEIGHT);
        viewXMLButton = new QRect(0, currentHeight - 2 * STATE_TAB_HEIGHT, STATE_TAB_WIDTH, STATE_TAB_HEIGHT);
        deleteButton = new QRect(0, currentHeight - 3 * STATE_TAB_HEIGHT, STATE_TAB_WIDTH, STATE_TAB_HEIGHT);
    }

    QPainter painter(this);
    painter.drawRect(*saveButton);
    painter.drawRect(*viewXMLButton);
    painter.drawRect(*deleteButton);
    painter.drawText(*saveButton, Qt::AlignCenter, "保存");
    painter.drawText(*viewXMLButton, Qt::AlignCenter, "查看XML");
    painter.drawText(*deleteButton, Qt::AlignCenter, "删除");
}

void CustomStateTabWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    QPoint pos = event->pos();
    if (saveButton->contains(pos)) {
        // TODO save state machine
    }
    else if (viewXMLButton->contains(pos)) {
        // TODO view xml
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

void CustomStateTabWidget::addCustomTab(int clickedTab) {
    if (clickedTab + 1 == this->count()) {
        if ((this->count() + 4) * STATE_TAB_HEIGHT > height()) {
            emit sendStatusMessage("无法添加更多状态机");
        }
        else {
            auto stateWidget = new CustomStateWidget(this);
            this->insertTab(clickedTab, stateWidget, (std::to_string(clickedTab + 1)).c_str());
            QObject::connect(stateWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
        }
    }
}