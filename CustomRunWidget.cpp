//
// Created by yingzi on 2017/10/6.
//

#include "CustomRunWidget.h"

CustomRunWidget::CustomRunWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);
    eventComboBox = new QComboBox(this);
    stateComboBox = new QComboBox(this);
    eventTextBrowser = new QTextBrowser(this);
    stateGraphicsView = new QGraphicsView(this);
    sourceIpEdit = new CustomIpEdit(this);
    destIpEdit = new CustomIpEdit(this);
    pseudoTerminalEdit = new QLineEdit(this);
    serialPortEdit = new QLineEdit(this);
    vmEdit = new QLineEdit(this);
    runButton = new QPushButton(this);
    eventTraceTextBrowser = new QTextBrowser(this);

    gridLayout->addWidget(eventComboBox, 0, 0, 1, 1);
    gridLayout->addWidget(stateComboBox, 0, 1, 1, 1);
    gridLayout->addWidget(eventTextBrowser);
    gridLayout->addWidget(stateGraphicsView);
    gridLayout->addWidget(sourceIpEdit);
    gridLayout->addWidget(destIpEdit);
    gridLayout->addWidget(pseudoTerminalEdit);
    gridLayout->addWidget(serialPortEdit);
    gridLayout->addWidget(vmEdit);
    gridLayout->addWidget(runButton);
    gridLayout->addWidget(eventTraceTextBrowser);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

}
