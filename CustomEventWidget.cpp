//
// Created by yingzi on 2017/10/6.
//

#include "CustomEventWidget.h"

CustomEventWidget::CustomEventWidget(QWidget *parent) : QWidget(parent) {
    upperOutLayer = new QTextEdit(this);
    middleLayer = new QTextEdit(this);
    bottomOutLayer = new QTextEdit(this);
    upperIndicator = new QTextBrowser(this);
    middleIndicator = new QTextBrowser(this);
    bottomIndicator = new QTextBrowser(this);

    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    gridLayout->addWidget(upperOutLayer, 0, 0, 1, 4);
    gridLayout->addWidget(upperIndicator, 0, 4, 1, 1);
    gridLayout->addWidget(middleLayer, 1, 0, 3, 4);
    gridLayout->addWidget(middleIndicator, 1, 4, 3, 1);
    gridLayout->addWidget(bottomOutLayer, 4, 0, 1, 4);
    gridLayout->addWidget(bottomIndicator, 4, 4, 1, 1);

    upperOutLayer->setText("upperOutLayer");
    middleLayer->setText("middleLayer");
    bottomOutLayer->setText("bottomOutLayer");
    upperIndicator->setText("upperIndicator");
    middleIndicator->setText("middleIndicator");
    bottomIndicator->setText("bottomIndicator");
}
