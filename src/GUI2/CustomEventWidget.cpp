//
// Created by chenkuan on 2018/2/20.
//

#include "CustomEventWidget.h"
#include <GUIConf.h>

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
    upperOutLayer->setText(DEFAULT_EVENT_MATCH_HEAD);
    middleLayer->setText(DEFAULT_EVENT_MATCH_BODY);
    bottomOutLayer->setText(DEFAULT_EVENT_MATCH_TAIL);
    upperIndicator->setText(EVENT_MATCH_HEAD_DESC);
    middleIndicator->setText(EVENT_MATCH_BODY_DESC);
    bottomIndicator->setText(EVENT_MATCH_TAIL_DESC);

    // 添加内容变化的响应
    QObject::connect(upperOutLayer, SIGNAL(textChanged()), this, SIGNAL(eventContentChanged()));
    QObject::connect(middleLayer, SIGNAL(textChanged()), this, SIGNAL(eventContentChanged()));
    QObject::connect(bottomOutLayer, SIGNAL(textChanged()), this, SIGNAL(eventContentChanged()));

}

QString CustomEventWidget::text() {
    return upperOutLayer->toPlainText()
           + '\n'
           + middleLayer->toPlainText()
           + '\n'
           + bottomOutLayer->toPlainText();
}
