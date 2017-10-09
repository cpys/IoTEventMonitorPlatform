//
// Created by yingzi on 2017/10/8.
//

#include "CustomSubTabWidget.h"

CustomSubTabWidget::CustomSubTabWidget(QWidget *parent) : QWidget(parent) {
    hBoxLayout = new QHBoxLayout(this);
    leftVBoxLayout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget(this);

    hBoxLayout->addLayout(leftVBoxLayout);
    hBoxLayout->addWidget(stackedWidget);

    listWidget = new QListWidget(this);
    listWidget->addItem("+");
    saveButton = new QPushButton("保存", this);
    deleteButton = new QPushButton("删除", this);

    leftVBoxLayout->addWidget(listWidget);
    leftVBoxLayout->addWidget(deleteButton);
    leftVBoxLayout->addWidget(saveButton);

    QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(changeListRow(int)));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentTab()));
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentTab()));
    this->changeTabStyle();
}

void CustomSubTabWidget::changeListRow(int row) {
    // 如果只是切换选项卡，当前的基类就可以完成了
    if (row != listWidget->count() - 1) {
        stackedWidget->setCurrentIndex(row);
    }
    // 否则调用虚函数，由子类完成添加选项卡
    else {
        addCustomTab();
        listWidget->current
        listWidget->setCurrentRow(listWidget->count() - 1);
    }
}

void CustomSubTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}

void CustomSubTabWidget::deleteCurrentTab() {
    int currentIndex = listWidget->currentRow();
    // 只要不为+号，就删除
    if (currentIndex >= 0 && currentIndex != listWidget->count() - 1) {
        emit sendStatusMessage("删除" + listWidget->item(currentIndex)->text());
        listWidget->removeItemWidget(listWidget->item(currentIndex));
        stackedWidget->removeWidget(stackedWidget->widget(currentIndex));
    }
}

void CustomSubTabWidget::changeTabStyle() {
    // TODO change list style
    listWidget->setFixedWidth(SUB_TAB_WIDTH);
}


