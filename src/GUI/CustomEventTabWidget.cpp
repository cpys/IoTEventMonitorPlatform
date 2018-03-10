//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include <string>
#include <iostream>
#include <QtWidgets/QMainWindow>
#include "CustomEventTabWidget.h"

CustomEventTabWidget::CustomEventTabWidget(QWidget *parent) : CustomSubTabWidget(parent) {}

void CustomEventTabWidget::setConf(XMLElement *eventsConf) {
    this->eventsConf = eventsConf;
    // 遍历所有<event>节点
    for (XMLElement *eventConf = eventsConf->FirstChildElement("event"); eventConf != nullptr; eventConf = eventConf->NextSiblingElement("event")) {
        // 添加到listWidget上
        listWidget->addItem(eventConf->Attribute("name"));
        // 添加到stackedWidget上
        auto customEventWidget = new CustomEventWidget(this);
        customEventWidget->setConf(eventConf);
        stackedWidget->addWidget(customEventWidget);
    }
    listWidget->setCurrentRow(0);
}

void CustomEventTabWidget::saveConfToXML() {
    // 先清空events下的所有event
    this->eventsConf->DeleteChildren();

    for (int row = 0; row < listWidget->count(); ++row) {
        // 插入一个event节点
        auto eventElement = eventsConf->InsertEndChild(eventsConf->GetDocument()->NewElement("event"))->ToElement();
        eventElement->SetAttribute("name", listWidget->item(row)->text().toStdString().c_str());
        // 让stackedWidget中的widget继续更新event节点
        dynamic_cast<CustomEventWidget*>(stackedWidget->widget(row))->saveConfToXML(eventElement);
    }
}

void CustomEventTabWidget::addCustomTab() {
    listWidget->addItem((EVENT + std::to_string(listWidget->count() + 1)).c_str());

    auto eventWidget = new CustomEventWidget(this);
    stackedWidget->addWidget(eventWidget);

    listWidget->setCurrentRow(listWidget->count() - 1);
    emit sendStatusMessage("添加" + listWidget->currentItem()->text());
}
