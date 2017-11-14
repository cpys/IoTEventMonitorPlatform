//
// Created by yingzi on 2017/10/6.
//

#include <QtWidgets/QFormLayout>
#include "CustomRunWidget.h"
#include "CustomEventWidget.h"

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

    runButton = new QPushButton("启动", this);

    eventManager = new EventManager(this);

    gridLayout->addWidget(eventLabel, 0, 0);
    gridLayout->addWidget(eventComboBox, 0, 1);
    gridLayout->addWidget(stateLabel, 0, 2);
    gridLayout->addWidget(stateComboBox, 0, 3);

    gridLayout->addWidget(eventTextBrowser, 1, 0, 1, 4);
    gridLayout->addWidget(stateGraphicsView, 2, 0, 1, 4);

    gridLayout->addWidget(eventTraceTextBrowser, 0, 4, 3, 5);

    gridLayout->addWidget(vmIpLabel, 3, 0);
    gridLayout->addWidget(externalIpLabel, 4, 0);
    gridLayout->addWidget(vmIpEdit, 3, 1, 1, 3);
    gridLayout->addWidget(externalIpEdit, 4, 1, 1, 3);

    gridLayout->addWidget(pseudoTerminalLabel, 3, 4);
    gridLayout->addWidget(serialPortLabel, 4, 4);
    gridLayout->addWidget(pseudoTerminalEdit, 3, 5);
    gridLayout->addWidget(serialPortEdit, 4, 5);

    gridLayout->addWidget(vmNameLabel, 3, 6);
    gridLayout->addWidget(vmPidLabel, 4, 6);
    gridLayout->addWidget(vmNameEdit, 3, 7);
    gridLayout->addWidget(vmPidEdit, 4, 7);

    gridLayout->addWidget(runButton, 3, 8, 2, 1);

    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    // 让下拉框与显示框对应
    QObject::connect(eventComboBox, SIGNAL(activated(int)), this, SLOT(showSelectEvent(int)));
    QObject::connect(stateComboBox, SIGNAL(activated(int)), this, SLOT(showSelectState(int)));

    // 启动按钮
    QObject::connect(runButton, SIGNAL(clicked()), this, SLOT(runButtonClicked()));

    // 日志展示，连接子线程函数
    QObject::connect(eventManager, SIGNAL(sendLogMessage(const QString&)), this, SLOT(showLogMessage(const QString&)));

    QObject::connect(eventManager, SIGNAL(sendStatusMessage(const QString&)), this, SLOT(recvStatusMessage(const QString&)));
    QObject::connect(eventManager, SIGNAL(finished()), this, SLOT(threadFinished()));
}

void CustomRunWidget::setConf(XMLElement *runConf) {
    this->runConf = runConf;

    eventComboBox->setCurrentIndex(eventComboBox->findText(runConf->Attribute("event")));
    stateComboBox->setCurrentIndex(stateComboBox->findText(runConf->Attribute("stateMachine")));

    vmIpEdit->setIp(runConf->Attribute("vmIP"));
    externalIpEdit->setIp(runConf->Attribute("externalIP"));

    pseudoTerminalEdit->setText(runConf->Attribute("pseudoTerminal"));
    serialPortEdit->setText(runConf->Attribute("serialPort"));

    vmNameEdit->setText(runConf->Attribute("vmName"));
    vmPidEdit->setText(runConf->Attribute("vmPID"));
}

void CustomRunWidget::saveConfToXML() {
    runConf->SetAttribute("event", eventComboBox->currentText().toStdString().c_str());
    runConf->SetAttribute("state", stateComboBox->currentText().toStdString().c_str());

    runConf->SetAttribute("vmIP", vmIpEdit->getIp().c_str());
    runConf->SetAttribute("externalIP", externalIpEdit->getIp().c_str());

    runConf->SetAttribute("pseudoTerminal", pseudoTerminalEdit->text().toStdString().c_str());
    runConf->SetAttribute("serialPort", serialPortEdit->text().toStdString().c_str());

    runConf->SetAttribute("vmName", vmNameEdit->text().toStdString().c_str());
    runConf->SetAttribute("vmPID", vmPidEdit->text().toStdString().c_str());
}

void CustomRunWidget::setEventList(const QListWidget *listWidget, const QStackedWidget *stackedWidget) {
    this->eventListWidget = listWidget;
    this->eventStackedWidget = stackedWidget;
}

void CustomRunWidget::setStateList(const QListWidget *listWidget, const QStackedWidget *stackedWidget) {
    this->stateListWidget = listWidget;
    this->stateStackedWidget = stackedWidget;
}

void CustomRunWidget::updateWidget() {
    updateEventList();
    updateStateList();
}

void CustomRunWidget::updateEventList() {
    // 暂存当前选择
    QString currentText = eventComboBox->currentText();

    // 清空后重新更新
    eventComboBox->clear();
    auto item = eventListWidget->item(0);
    for (int row = 0; row < eventListWidget->count(); ++row, item = eventListWidget->item(row)) {
        eventComboBox->addItem(item->text());
    }

    int lastIndex = eventComboBox->findText(currentText);
    if (lastIndex < 0) {
        eventComboBox->setCurrentIndex(0);
        showSelectEvent(0);
    }
    else {
        eventComboBox->setCurrentIndex(lastIndex);
        showSelectEvent(lastIndex);
    }
}

void CustomRunWidget::updateStateList() {
    // 暂存当前选择
    QString currentText = eventComboBox->currentText();

    // 清空后重新更新
    stateComboBox->clear();
    auto item = stateListWidget->item(0);
    for (int row = 0; row < stateListWidget->count(); ++row, item = stateListWidget->item(row)) {
        stateComboBox->addItem(item->text());
    }

    int lastIndex = stateComboBox->findText(currentText);
    if (lastIndex < 0) {
        stateComboBox->setCurrentIndex(0);
        showSelectState(0);
    }
    else {
        stateComboBox->setCurrentIndex(lastIndex);
        showSelectState(lastIndex);
    }
}

void CustomRunWidget::run() {
    // 开始执行后更改各组件状态
    changeWidgetState(false);
    runButton->setText("停止");

    // 启动处理线程
    sendStatusMessage("正在启动...");
    eventManager->setEventConf(currentEventWidget->getHeadText(), currentEventWidget->getBodyText(), currentEventWidget->getTailText());
    eventManager->setNetfilterConf(this->vmIpEdit->getIp(), this->externalIpEdit->getIp());
    eventManager->start();
}

void CustomRunWidget::stop() {
    // 结束处理线程
    eventManager->stop();
    sendStatusMessage("正在停止...");
    eventManager->wait();
    sendStatusMessage("事件接收处理已停止");
}

void CustomRunWidget::changeWidgetState(bool isEnabled) {
    eventComboBox->setEnabled(isEnabled);
    stateComboBox->setEnabled(isEnabled);
    vmIpEdit->setEnabled(isEnabled);
    externalIpEdit->setEnabled(isEnabled);
    pseudoTerminalEdit->setEnabled(isEnabled);
    serialPortEdit->setEnabled(isEnabled);
    vmNameEdit->setEnabled(isEnabled);
    vmPidEdit->setEnabled(isEnabled);
}

void CustomRunWidget::runButtonClicked() {
    // 判断按钮状态调用不同的函数
    if (runButton->text() == "启动") {
        run();
    } else {
        stop();
    }
}

void CustomRunWidget::recvStatusMessage(const QString &message) {
    emit sendStatusMessage(message);
}

void CustomRunWidget::showSelectEvent(int index) {
    currentEventWidget = dynamic_cast<CustomEventWidget*>(eventStackedWidget->widget(index));
    eventTextBrowser->setText(currentEventWidget->getFullText().c_str());
}

void CustomRunWidget::showSelectState(int index) {
    // TODO show selected state
}

void CustomRunWidget::threadFinished() {
    // 结束执行后更改各组件状态
    changeWidgetState(true);
    runButton->setText("启动");
}

void CustomRunWidget::showLogMessage(const QString &message) {
    this->eventTraceTextBrowser->insertPlainText(message + "\n");
    this->eventTraceTextBrowser->moveCursor(QTextCursor::End);
}


