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
    listWidget->insertItem(0, "事件1");
    listWidget->setCurrentRow(0);

    auto eventWidget = new CustomEventWidget(this);
    stackedWidget->addWidget(eventWidget);

    QObject::connect(eventWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
}

void CustomEventTabWidget::addCustomTab() {
    listWidget->insertItem(listWidget->count() - 1, ("事件" + std::to_string(listWidget->count())).c_str());

    auto eventWidget = new CustomEventWidget(this);
    stackedWidget->addWidget(eventWidget);
}

void CustomEventTabWidget::saveCurrentTab() {
    // TODO save event
}
