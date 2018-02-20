//
// Created by chenkuan on 2018/2/20.
//

#include "CustomEventWidget.h"

CustomEventWidget::CustomEventWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    upperOutLayer = new QTextEdit(this);
    middleLayer = new QTextEdit(this);
    bottomOutLayer = new QTextEdit(this);
    upperIndicator = new QTextBrowser(this);
    middleIndicator = new QTextBrowser(this);
    bottomIndicator = new QTextBrowser(this);

    // 完成网格布局
    gridLayout->addWidget(upperOutLayer, 0, 0);
    gridLayout->addWidget(middleLayer, 1, 0);
    gridLayout->addWidget(bottomOutLayer, 2, 0);
    gridLayout->addWidget(upperIndicator, 0, 1);
    gridLayout->addWidget(middleIndicator, 1, 1);
    gridLayout->addWidget(bottomIndicator, 2, 1);

    // 设置布局比例
    gridLayout->setColumnStretch(0, 9);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setRowStretch(0, 2);
    gridLayout->setRowStretch(1, 6);
    gridLayout->setRowStretch(2, 2);

    // 设置间距风格
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    // 设置默认内容
    upperOutLayer->setText("<xml>");
    middleLayer->setText("<name>value</name>");
    bottomOutLayer->setText("</xml>");
    upperIndicator->setText("事件匹配头部");
    middleIndicator->setText("事件非匹配主体");
    bottomIndicator->setText("事件匹配尾部");
}