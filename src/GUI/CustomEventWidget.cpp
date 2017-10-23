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
    gridLayout->addWidget(upperOutLayer,0, 0);
    gridLayout->addWidget(middleLayer, 1, 0);
    gridLayout->addWidget(bottomOutLayer, 2, 0);
    gridLayout->addWidget(upperIndicator, 0, 1);
    gridLayout->addWidget(middleIndicator, 1, 1);
    gridLayout->addWidget(bottomIndicator, 2, 1);
    gridLayout->setColumnStretch(0, 9);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setRowStretch(0, 2);
    gridLayout->setRowStretch(1, 6);
    gridLayout->setRowStretch(2, 2);

    upperOutLayer->setText("<?xml>\n<schema xmlns:tns=\"...\", xmlns=\"...\",targetNamespace=\"...\">");
    middleLayer->setText("<name>value</name>\n...");
    bottomOutLayer->setText("</schema>\n</xml>");
    upperIndicator->setText("<- 锁定外层");
    middleIndicator->setText("<- 浮动内层");
    bottomIndicator->setText("<- 锁定外层");

    upperIndicator->setAlignment(Qt::AlignCenter|Qt::AlignBottom|Qt::AlignHCenter);
}

void CustomEventWidget::setConf(XMLElement *eventConf) {
    auto headConf = eventConf->FirstChildElement("head");
    auto bodyConf = eventConf->FirstChildElement("body");
    auto tailConf = eventConf->FirstChildElement("tail");

    if (headConf) upperOutLayer->setText(headConf->GetText());
    if (bodyConf) middleLayer->setText(bodyConf->GetText());
    if (tailConf) bottomOutLayer->setText(tailConf->GetText());
}

void CustomEventWidget::saveConfToXML(XMLElement *eventElement) {
    auto head = eventElement->InsertEndChild(eventElement->GetDocument()->NewElement("head"))->ToElement();
    auto body = eventElement->InsertEndChild(eventElement->GetDocument()->NewElement("body"))->ToElement();
    auto tail = eventElement->InsertEndChild(eventElement->GetDocument()->NewElement("tail"))->ToElement();

    head->SetText(upperOutLayer->toPlainText().toStdString().c_str());
    body->SetText(middleLayer->toPlainText().toStdString().c_str());
    tail->SetText(bottomOutLayer->toPlainText().toStdString().c_str());
}


