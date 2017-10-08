//
// Created by yingzi on 2017/10/6.
//

#include <QtWidgets/QFormLayout>
#include "CustomRunWidget.h"

CustomRunWidget::CustomRunWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);

    eventComboBox = new QComboBox(this);
    stateComboBox = new QComboBox(this);
    eventLabel = new QLabel("事件选择", this);
    stateLabel = new QLabel("状态机选择", this);

    eventTextBrowser = new QTextBrowser(this);
    stateGraphicsView = new QGraphicsView(this);
    eventTextBrowser->setText("事件模板预览");
    eventTextBrowser->setAlignment(Qt::AlignCenter);

    vmIpEdit = new CustomIpEdit(this);
    hostIpEdit = new CustomIpEdit(this);
    externalIpEdit = new CustomIpEdit(this);
    vmIpLabel = new QLabel("虚拟机ip", this);
    hostIpLabel = new QLabel("宿主机ip", this);
    externalIpLabel = new QLabel("外部设备ip", this);

    pseudoTerminalEdit = new QLineEdit(this);
    serialPortEdit = new QLineEdit(this);
    pseudoTerminalLabel = new QLabel("KVM伪终端名称", this);
    serialPortLabel = new QLabel("宿主机串口名称", this);

    vmNameEdit = new QLineEdit(this);
    vmPidEdit = new QLineEdit(this);
    vmNameLabel = new QLabel("虚拟机名称", this);
    vmPidLabel = new QLabel("虚拟机进程pid", this);

    runButton = new QPushButton("启动", this);
    eventTraceTextBrowser = new QTextBrowser(this);

    gridLayout->addWidget(eventComboBox, 0, 0);
    gridLayout->addWidget(stateComboBox, 1, 0);
    gridLayout->addWidget(eventLabel, 0, 1);
    gridLayout->addWidget(stateLabel, 1, 1);

    gridLayout->addWidget(eventTextBrowser, 2, 0, 1, 2);
    gridLayout->addWidget(stateGraphicsView, 0, 2, 4, 4);

    gridLayout->addWidget(vmIpEdit, 3, 0);
    gridLayout->addWidget(hostIpEdit, 4, 0);
    gridLayout->addWidget(externalIpEdit, 5, 0);
    gridLayout->addWidget(vmIpLabel, 3, 1);
    gridLayout->addWidget(hostIpLabel, 4, 1);
    gridLayout->addWidget(externalIpLabel, 5, 1);

    gridLayout->addWidget(pseudoTerminalEdit, 4, 2);
    gridLayout->addWidget(serialPortEdit, 5, 2);
    gridLayout->addWidget(pseudoTerminalLabel, 4, 3);
    gridLayout->addWidget(serialPortLabel, 5, 3);

    gridLayout->addWidget(vmNameEdit, 4, 4);
    gridLayout->addWidget(vmPidEdit, 5, 4);
    gridLayout->addWidget(vmNameLabel, 4, 5);
    gridLayout->addWidget(vmPidLabel, 5, 5);

    gridLayout->addWidget(runButton, 0, 6);
    gridLayout->addWidget(eventTraceTextBrowser, 1, 6, 5, 1);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
}
