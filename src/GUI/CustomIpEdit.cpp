//
// Created by yingzi on 2017/10/8.
//

#include "CustomIpEdit.h"

CustomIpEdit::CustomIpEdit(QWidget *parent) : QWidget(parent) {
    hBoxLayout = new QHBoxLayout(this);
    for (auto& ipEdit : ipEdits) {
        ipEdit = new QLineEdit("0", this);
    }
    for (auto& pointLabel : pointLabels) {
        pointLabel = new QLabel(".", this);
    }

    for (int i = 0; i < 3; ++i) {
        hBoxLayout->addWidget(ipEdits[i]);
        hBoxLayout->addWidget(pointLabels[i]);
    }
    hBoxLayout->addWidget(ipEdits.back());
    this->changeStyle();

//    hBoxLayout->addWidget(new QLineEdit("0", this));
//    hBoxLayout->addWidget(new QLabel(".", this));
//    hBoxLayout->addWidget(new QLineEdit("0", this));
}

void CustomIpEdit::changeStyle() {
    this->setMaximumWidth(200);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    for (auto& ipEdit : ipEdits) {
//        ipEdit->setMinimumWidth(10);
//        ipEdit->setMaximumWidth(20);
//        ipEdit->setFixedWidth(10);
        ipEdit->setContentsMargins(0, 0, 0, 0);
    }
    for (auto& pointLabel : pointLabels) {
        pointLabel->setMaximumWidth(3);
        pointLabel->setContentsMargins(0, 0, 0, 0);
    }
}
