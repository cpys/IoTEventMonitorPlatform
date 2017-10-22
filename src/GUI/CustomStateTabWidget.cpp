//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include "CustomStateTabWidget.h"

CustomStateTabWidget::CustomStateTabWidget(QWidget *parent) : CustomSubTabWidget(parent) {
    listWidget->addItem("状态机1");

    auto stateWidget = new CustomStateWidget(this);
    stackedWidget->addWidget(stateWidget);

    viewXMLButton = new QPushButton("查看XML", this);
    leftVBoxLayout->addWidget(viewXMLButton);

    QObject::connect(stateWidget, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
    QObject::connect(viewXMLButton, SIGNAL(clicked()), this, SLOT(viewCurrentXML()));
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

void CustomStateTabWidget::viewCurrentXML() {
    // TODO show current xml
}
