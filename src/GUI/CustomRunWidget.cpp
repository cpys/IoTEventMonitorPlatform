//
// Created by yingzi on 2017/10/6.
//

#include <QtWidgets/QFormLayout>
#include "CustomRunWidget.h"

CustomRunWidget::CustomRunWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);

    eventLabel = new QLabel("事件选择", this);
    stateLabel = new QLabel("状态机选择", this);
    eventComboBox = new QComboBox(this);
    stateComboBox = new QComboBox(this);

    eventTextBrowser = new QTextBrowser(this);
    stateGraphicsView = new QGraphicsView(this);
    eventTextBrowser->setText("事件模板预览");
    eventTextBrowser->setAlignment(Qt::AlignCenter);

    eventTraceTextBrowser = new QTextBrowser(this);

    vmIpLabel = new QLabel("虚拟机ip", this);
    externalIpLabel = new QLabel("外部设备ip", this);
    vmIpEdit = new CustomIpEdit(this);
    externalIpEdit = new CustomIpEdit(this);

    pseudoTerminalLabel = new QLabel("KVM伪终端名称", this);
    serialPortLabel = new QLabel("宿主机串口名称", this);
    pseudoTerminalEdit = new QLineEdit(this);
    serialPortEdit = new QLineEdit(this);

    vmNameLabel = new QLabel("虚拟机名称", this);
    vmPidLabel = new QLabel("虚拟机进程pid", this);
    vmNameEdit = new QLineEdit(this);
    vmPidEdit = new QLineEdit(this);

    hostIpLabel = new QLabel("宿主机ip", this);
    hostIpEdit = new CustomIpEdit(this);

    runButton = new QPushButton("启动", this);

    gridLayout->addWidget(eventLabel, 0, 0);
    gridLayout->addWidget(eventComboBox, 0, 1);
    gridLayout->addWidget(stateLabel, 0, 2);
    gridLayout->addWidget(stateComboBox, 0, 3);

    gridLayout->addWidget(eventTextBrowser, 1, 0, 1, 4);
    gridLayout->addWidget(stateGraphicsView, 2, 0, 1, 4);

    gridLayout->addWidget(eventTraceTextBrowser, 0, 4, 3, 4);

    gridLayout->addWidget(vmIpLabel, 3, 0);
    gridLayout->addWidget(externalIpLabel, 4, 0);
    gridLayout->addWidget(vmIpEdit, 3, 1);
    gridLayout->addWidget(externalIpEdit, 4, 1);

    gridLayout->addWidget(pseudoTerminalLabel, 3, 2);
    gridLayout->addWidget(serialPortLabel, 4, 2);
    gridLayout->addWidget(pseudoTerminalEdit, 3, 3);
    gridLayout->addWidget(serialPortEdit, 4, 3);

    gridLayout->addWidget(vmNameLabel, 3, 4);
    gridLayout->addWidget(vmPidLabel, 4, 4);
    gridLayout->addWidget(vmNameEdit, 3, 5);
    gridLayout->addWidget(vmPidEdit, 4, 5);

    gridLayout->addWidget(hostIpLabel, 3, 6);
    gridLayout->addWidget(hostIpEdit, 3, 7);

    gridLayout->addWidget(runButton, 4, 6, 1, 2);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
}

void CustomRunWidget::setConf(XMLElement *runConf) {
    vmIpEdit->setIp(runConf->Attribute("vmIP"));
    externalIpEdit->setIp(runConf->Attribute("externalIP"));

    pseudoTerminalEdit->setText(runConf->Attribute("pseudoTerminal"));
    serialPortEdit->setText(runConf->Attribute("serialPort"));

    vmNameEdit->setText(runConf->Attribute("vmName"));
    vmPidEdit->setText(runConf->Attribute("vmPID"));

    hostIpEdit->setIp(runConf->Attribute("hostIP"));
}

void CustomRunWidget::saveConfToXML() {
    runConf->SetAttribute("vmIP", vmIpEdit->getIp().c_str());
    runConf->SetAttribute("externalIP", externalIpEdit->getIp().c_str());

    runConf->SetAttribute("pseudoTerminal", pseudoTerminalEdit->text().toStdString().c_str());
    runConf->SetAttribute("serialPort", serialPortEdit->text().toStdString().c_str());

    runConf->SetAttribute("vmName", vmNameEdit->text().toStdString().c_str());
    runConf->SetAttribute("vmPID", vmPidEdit->text().toStdString().c_str());

    runConf->SetAttribute("hostIP", hostIpEdit->getIp().c_str());
}

void CustomRunWidget::setEventList(const QListWidget *listWidget, const QStackedWidget *stackedWidget) {
    this->eventListWidget = listWidget;
    this->eventStackedWidget = stackedWidget;
}

void CustomRunWidget::setStateList(const QListWidget *listWidget, const QStackedWidget *stackedWidget) {
    this->stateListWidget = listWidget;
    this->stateStackedWidget = stackedWidget;
}

void CustomRunWidget::updateEventList() {
    // TODO reread
    eventComboBox->clear();

    auto item = eventListWidget->item(0);
    for (int row = 0; row < eventListWidget->count(); ++row, item = eventListWidget->item(row)) {
        eventComboBox->addItem(item->text());
    }
}

void CustomRunWidget::updateStateList() {
    // TODO reread
    stateComboBox->clear();

    auto item = stateListWidget->item(0);
    for (int row = 0; row < stateListWidget->count(); ++row, item = stateListWidget->item(row)) {
        stateComboBox->addItem(item->text());
    }
}



