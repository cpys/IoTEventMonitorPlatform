//
// Created by yingzi on 2017/10/6.
//

#include "CustomStateWidget.h"

CustomStateWidget::CustomStateWidget(QWidget *parent) : QWidget(parent) {
    vBoxLayout = new QHBoxLayout(this);
    canvas = new QGraphicsView(this);
    graphBoard = new QGraphicsView(this);

    vBoxLayout->addWidget(canvas, 8);
    vBoxLayout->addWidget(graphBoard, 2);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
}
