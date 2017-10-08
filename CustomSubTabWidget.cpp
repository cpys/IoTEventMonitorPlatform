//
// Created by yingzi on 2017/10/8.
//

#include "CustomSubTabWidget.h"

CustomSubTabWidget::CustomSubTabWidget(QWidget *parent) : QWidget(parent) {
    vBoxLayout = new QVBoxLayout(this);
    leftHBoxLayout = new QHBoxLayout(this);
    stackedWidget = new QStackedWidget(this);

    vBoxLayout->addLayout(leftHBoxLayout);
    vBoxLayout->addWidget(stackedWidget);

    listWidget = new QListWidget(this);
    listWidget->addItem("+");
    saveButton = new QPushButton(this);
    deleteButton = new QPushButton(this);

    leftHBoxLayout->addWidget(listWidget);
    leftHBoxLayout->addWidget(deleteButton);
    leftHBoxLayout->addWidget(saveButton);

    QObject::connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickListWidget(QListWidgetItem*)));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentTab()));
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentTab()));
    this->changeTabStyle();
}

void CustomSubTabWidget::clickListWidget(QListWidgetItem *clickedListWidgetItem) {
    // 如果只是切换选项卡，当前的基类就可以完成了
    if (clickedListWidgetItem != listWidget->item(listWidget->count() - 1)) {
        listWidget->setCurrentItem(clickedListWidgetItem);
    }
    // 否则调用虚函数，由子类完成添加选项卡
    else {
        addCustomTab();
    }
}

void CustomSubTabWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}

void CustomSubTabWidget::deleteCurrentTab() {
    int currentIndex = listWidget->currentIndex().row();
    // TODO 确认未选中时默认为多少
    // 只要不为+号，就删除
    if (currentIndex != listWidget->count() - 1) {
        listWidget->removeItemWidget(listWidget->item(currentIndex));
        stackedWidget->removeWidget(stackedWidget->widget(currentIndex));
    }
}

void CustomSubTabWidget::changeTabStyle() {
    // TODO change list style
    this->setStyleSheet((
                                "QTabWidget::pane {"
                                        "border: none;"
                                        "}"
                                        "QTabWidget::tab-bar {"
                                        "border: none;"
                                        "}"
                                        "QTabBar::tab {"
                                        "min-width: " + std::to_string(EVENT_TAB_WIDTH) + "px;"
                                        "height: " + std::to_string(EVENT_TAB_HEIGHT) + "px;"
                                        "}"
                        ).c_str());
}


