//
// Created by yingzi on 2017/10/8.
//

#include <iostream>
#include "CustomSubTabWidget.h"

CustomSubTabWidget::CustomSubTabWidget(QWidget *parent) : QWidget(parent) {
    hBoxLayout = new QHBoxLayout(this);
    leftVBoxLayout = new QVBoxLayout();
    stackedWidget = new QStackedWidget(this);

    hBoxLayout->addLayout(leftVBoxLayout);
    hBoxLayout->addWidget(stackedWidget);

    listWidget = new QListWidget(this);
    saveButton = new QPushButton("保存", this);
    deleteButton = new QPushButton("-", this);
    addButton = new QPushButton("+", this);

    leftVBoxLayout->addWidget(listWidget);
    leftVBoxLayout->addWidget(addButton);
    leftVBoxLayout->addWidget(deleteButton);
    leftVBoxLayout->addWidget(saveButton);

    QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
    QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(addCustomTab()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentTab()));
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentTab()));
    this->changeTabStyle();
}

const QListWidget *CustomSubTabWidget::getListWidget() {
    return listWidget;
}

const QStackedWidget *CustomSubTabWidget::getStackedWidget() {
    return stackedWidget;
}

void CustomSubTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}

void CustomSubTabWidget::deleteCurrentTab() {

    // 排除当前未选择任一item的情况
    int currentIndex = listWidget->currentRow();
    if (currentIndex < 0) return;

    emit sendStatusMessage("删除" + listWidget->item(currentIndex)->text());
    delete listWidget->takeItem(currentIndex);

    QWidget *currentStackedWidget = stackedWidget->widget(currentIndex);
    stackedWidget->removeWidget(currentStackedWidget);
    delete currentStackedWidget;

    // 发射页面变更信号
    emit listChanged();
}

void CustomSubTabWidget::changeTabStyle() {
    // TODO change list style
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    leftVBoxLayout->setContentsMargins(0, 0, 0, 0);
    leftVBoxLayout->setSpacing(0);

    listWidget->setFixedWidth(SUB_TAB_WIDTH);
    listWidget->setContentsMargins(0, 0, 0, 0);
    stackedWidget->setContentsMargins(0, 0, 0, 0);

    addButton->setContentsMargins(0, 0, 0, 0);
    deleteButton->setContentsMargins(0, 0, 0, 0);
    saveButton->setContentsMargins(0, 0, 0, 0);
}


