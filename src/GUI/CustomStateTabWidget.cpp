//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include "CustomStateTabWidget.h"

CustomStateTabWidget::CustomStateTabWidget(QWidget *parent) : CustomSubTabWidget(parent) {
    viewXMLButton = new QPushButton("查看XML", this);
    leftVBoxLayout->addWidget(viewXMLButton);

    QObject::connect(viewXMLButton, SIGNAL(clicked()), this, SLOT(viewCurrentXML()));
}

void CustomStateTabWidget::setConf(XMLElement *stateMachinesConf) {
    this->stateMachinesConf = stateMachinesConf;
    // 遍历所有<stateMachine>节点
    for (XMLElement *stateMachineConf = stateMachinesConf->FirstChildElement("stateMachine"); stateMachineConf != nullptr; stateMachineConf = stateMachineConf->NextSiblingElement("stateMachine")) {
        // 添加到listWidget上
        listWidget->addItem(stateMachineConf->Attribute("name"));
        // 添加到stackedWidget上
        auto customStateWidget = new CustomStateWidget(this);
        customStateWidget->setConf(stateMachineConf);
        stackedWidget->addWidget(customStateWidget);
    }

    // 发射页面变更信号
    emit listChanged();
}

void CustomStateTabWidget::saveConfToXML() {
    // 先清空stateMachines下的所有stateMachine
    this->stateMachinesConf->DeleteChildren();

    for (int row = 0; row < listWidget->count(); ++row) {
        // 插入一个stateMachine节点
        auto stateMachineElement = stateMachinesConf->InsertEndChild(stateMachinesConf->GetDocument()->NewElement("stateMachine"))->ToElement();
        stateMachineElement->SetAttribute("name", listWidget->item(row)->text().toStdString().c_str());
        // 让stackedWidget中的widget继续更新stateMachine节点
        dynamic_cast<CustomStateWidget*>(stackedWidget->widget(row))->saveConfToXML(stateMachineElement);
    }
}

void CustomStateTabWidget::addCustomTab() {
    listWidget->addItem(("状态机" + std::to_string(listWidget->count() + 1)).c_str());

    auto stateWidget = new CustomStateWidget(this);
    stackedWidget->addWidget(stateWidget);

    listWidget->setCurrentRow(listWidget->count() - 1);
    emit sendStatusMessage("添加" + listWidget->currentItem()->text());

    // 发射页面变更信号
    emit listChanged();
}

void CustomStateTabWidget::saveCurrentTab() {
    // TODO save state machine
}

void CustomStateTabWidget::viewCurrentXML() {
    // TODO show current xml
}
