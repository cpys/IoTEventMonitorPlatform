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
    listWidget->addItem("事件1");

    auto eventWidget = new CustomEventWidget(this);
    stackedWidget->addWidget(eventWidget);

    QObject::connect(eventWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
}

void CustomEventTabWidget::setConf(XMLElement *conf) {
    // TODO
}

void CustomEventTabWidget::addCustomTab() {
    listWidget->addItem(("事件" + std::to_string(listWidget->count() + 1)).c_str());

    auto eventWidget = new CustomEventWidget(this);
    stackedWidget->addWidget(eventWidget);

    listWidget->setCurrentRow(listWidget->count() - 1);
}

void CustomEventTabWidget::saveCurrentTab() {
    // TODO save event
}
