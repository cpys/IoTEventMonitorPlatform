//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include "CustomStateTabWidget.h"

CustomStateTabWidget::CustomStateTabWidget(QWidget *parent) : CustomSubTabWidget(parent) {
    listWidget->insertItem(0, "状态机1");
    listWidget->setCurrentRow(0);

    auto stateWidget = new CustomStateWidget(this);
    stackedWidget->addWidget(stateWidget);

    QObject::connect(stateWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
}

void CustomStateTabWidget::addCustomTab() {
    listWidget->addItem(("状态机" + std::to_string(listWidget->count() + 1)).c_str());

    auto stateWidget = new CustomStateWidget(this);
    stackedWidget->addWidget(stateWidget);

    listWidget->setCurrentRow(listWidget->count() - 1);
}

void CustomStateTabWidget::saveCurrentTab() {
    // TODO save state machine
}