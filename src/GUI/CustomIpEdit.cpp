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

    // TODO 对ip框加入输入校验器
//    for (auto& ipEdit : ipEdits) {
//        ipEdit->setValidator(new QValidator());
//    }

    for (int i = 0; i < 3; ++i) {
        hBoxLayout->addWidget(ipEdits[i]);
        hBoxLayout->addWidget(pointLabels[i]);
    }
    hBoxLayout->addWidget(ipEdits.back());
    this->changeStyle();
}


void CustomIpEdit::setIp(const string &ip) {
    QString ipStr(ip.c_str());
    QStringList ipStrList = ipStr.split(".");
    for (int i = 0; i < 4; ++i) {
        ipEdits[i]->setText(std::to_string(ipStrList[i].toInt()).c_str());
    }
}

string CustomIpEdit::getIp() {
    return ipEdits[0]->text().toStdString() +
           "." + ipEdits[1]->text().toStdString() +
                 "." + ipEdits[2]->text().toStdString() +
                       "." + ipEdits[3]->text().toStdString();
}

void CustomIpEdit::changeStyle() {
    this->setMaximumWidth(200);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    for (auto& ipEdit : ipEdits) {
        ipEdit->setContentsMargins(0, 0, 0, 0);
    }
    for (auto& pointLabel : pointLabels) {
        pointLabel->setMaximumWidth(3);
        pointLabel->setContentsMargins(0, 0, 0, 0);
    }
}