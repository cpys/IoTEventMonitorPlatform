//
// Created by chenkuan on 2018/2/20.
//

#include "CustomRunWidget.h"
#include <GUIConf.h>
#include <QtWidgets/QFileDialog>

CustomRunWidget::CustomRunWidget(QWidget *parent) : QWidget(parent) {
    hBoxLayout = new QHBoxLayout(this);
    leftGridLayout = new QGridLayout();
    logTextBrowser = new QTextBrowser(this);

    // 完成左右布局
    hBoxLayout->addLayout(leftGridLayout, 2);
    hBoxLayout->addWidget(logTextBrowser, 8);

    eventLabel = new QLabel(EVENT_SELECT_LABEL, this);
    eventComboBox = new QComboBox(this);

    eventPreviewStackedWidget = new QStackedWidget(this);

    vmIpLabel = new QLabel(VM_IP_LABEL, this);
    vmIpEdit = new CustomIpEdit(this);

    externalIpLabel = new QLabel(EXTERNAL_IP_LABEL, this);
    externalIpEdit = new CustomIpEdit(this);

    pseudoTerminalLabel = new QLabel(PSEUDO_TERMINAL_LABEL, this);
    pseudoTerminalEdit = new QLineEdit(this);

    serialPortLabel = new QLabel(HOST_SERIAL_PORT_LABEL, this);
    serialPortEdit = new QLineEdit(this);

    stateMachineSelectLabel = new QLabel(STATE_MACHINE_SELECT_LABEL, this);
    stateMachineEdit = new CustomLineEdit(this);

    runButton = new QPushButton(RUN_BUTTON, this);
    clearButton = new QPushButton(CLEAR_BUTTON, this);

    // 完成左侧的网格布局
    leftGridLayout->addWidget(eventLabel, 0, 0);
    leftGridLayout->addWidget(eventComboBox, 0, 1);

    leftGridLayout->addWidget(eventPreviewStackedWidget, 1, 0, 1, 2);

    leftGridLayout->addWidget(vmIpLabel, 2, 0);
    leftGridLayout->addWidget(vmIpEdit, 2, 1);

    leftGridLayout->addWidget(externalIpLabel, 3, 0);
    leftGridLayout->addWidget(externalIpEdit, 3, 1);

    leftGridLayout->addWidget(pseudoTerminalLabel, 4, 0);
    leftGridLayout->addWidget(pseudoTerminalEdit, 4, 1);

    leftGridLayout->addWidget(serialPortLabel, 5, 0);
    leftGridLayout->addWidget(serialPortEdit, 5, 1);

    leftGridLayout->addWidget(stateMachineSelectLabel, 6, 0);
    leftGridLayout->addWidget(stateMachineEdit, 6, 1);

    leftGridLayout->addWidget(runButton, 7, 0, 1, 2);
    leftGridLayout->addWidget(clearButton, 8, 0, 1, 2);

    // 设置间距风格
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    leftGridLayout->setContentsMargins(0, 0, 0, 0);
    leftGridLayout->setSpacing(0);

    // 设置下拉框与预览框对应
    QObject::connect(eventComboBox, SIGNAL(activated(int)),
                     eventPreviewStackedWidget, SLOT(setCurrentIndex(int)));

    // 设置状态机文件输入框点击的响应
    QObject::connect(stateMachineEdit, SIGNAL(clicked()),
                     this, SLOT(selectStateMachineFile()));

}

void CustomRunWidget::loadConf(XMLElement *runConf) {
    this->runConf = runConf;

    int currentEventIndex = eventComboBox->findText(runConf->Attribute(CURRENT_EVENT_NAME_ATTR));
    eventComboBox->setCurrentIndex(currentEventIndex);
    eventPreviewStackedWidget->setCurrentIndex(currentEventIndex);

    vmIpEdit->setIp(runConf->Attribute(VM_IP_ATTR));
    externalIpEdit->setIp(runConf->Attribute(EXTERNAL_IP_ATTR));
    pseudoTerminalEdit->setText(runConf->Attribute(PSEUDO_TERMINAL_ATTR));
    serialPortEdit->setText(runConf->Attribute(SERIAL_PORT_ATTR));
    stateMachineEdit->setText(runConf->Attribute(STATE_MACHINE_FILE_PATH_ATTR));
}

void CustomRunWidget::saveConf() {
    runConf->SetAttribute(CURRENT_EVENT_NAME_ATTR, eventComboBox->currentText().toStdString().c_str());
    runConf->SetAttribute(VM_IP_ATTR, vmIpEdit->getIp().toStdString().c_str());
    runConf->SetAttribute(EXTERNAL_IP_ATTR, externalIpEdit->getIp().toStdString().c_str());
    runConf->SetAttribute(PSEUDO_TERMINAL_ATTR, pseudoTerminalEdit->text().toStdString().c_str());
    runConf->SetAttribute(SERIAL_PORT_ATTR, serialPortEdit->text().toStdString().c_str());
    runConf->SetAttribute(STATE_MACHINE_FILE_PATH_ATTR, stateMachineEdit->text().toStdString().c_str());
}

void CustomRunWidget::insertEvent(int index, const QString &eventName, const QString &eventContent) {
    eventComboBox->insertItem(index, eventName);

    auto eventPreviewTextBrowser = new QTextBrowser(this);
    eventPreviewTextBrowser->setText(eventContent);
    eventPreviewStackedWidget->insertWidget(index, eventPreviewTextBrowser);
}

void CustomRunWidget::removeEvent(int index) {
    eventComboBox->removeItem(index);

    auto eventPreviewTextBrowser = eventPreviewStackedWidget->widget(index);
    eventPreviewStackedWidget->removeWidget(eventPreviewTextBrowser);
    delete (eventPreviewTextBrowser);
}

void CustomRunWidget::modifyEvent(int index, const QString &eventName, const QString &eventContent) {
    eventComboBox->setItemText(index, eventName);
    auto eventPreviewTextBrowser = dynamic_cast<QTextBrowser *>(eventPreviewStackedWidget->widget(index));
    eventPreviewTextBrowser->setText(eventContent);
}

void CustomRunWidget::selectStateMachineFile() {
    QString stateMachineFilePath = QFileDialog::getOpenFileName(this,
                                                                tr(SELECT_FILE_DIALOG_TITLE),
                                                                ".",
                                                                tr("XML files (*.xml)"));
    if (!stateMachineFilePath.isEmpty()) {
        stateMachineEdit->setText(stateMachineFilePath);
    }
}
