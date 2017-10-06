//
// Created by yingzi on 2017/10/6.
//

#include "CustomStateTabWidget.h"

CustomStateTabWidget::CustomStateTabWidget(QWidget *parent) : QTabWidget(parent) {
    addStateTabButton = new CustomStateWidget(parent);
    this->addTab(addStateTabButton, "+");
    this->setTabPosition(West);

    QObject::connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(addStateTab(int)));
}

void CustomStateTabWidget::paintEvent(QPaintEvent *event) {
    QTabWidget::paintEvent(event);
}

void CustomStateTabWidget::addStateTab(int clickedTab) {
    if (clickedTab + 1 == this->count()) {
        this->insertTab(clickedTab, new CustomStateWidget((QWidget*)this->parent()), ( std::to_string(clickedTab + 1)).c_str());
    }
}
