//
// Created by yingzi on 2017/11/8.
//

#include "EventManager.h"

EventManager::EventManager(QObject *parent) : QThread(parent) {
    netfilterClient = new NetfilterClient();
    internalClient = new SerialPortClient();
    externalClient = new SerialPortClient();

    stateParser = new StateParser();
}

EventManager::~EventManager() {
    stop();
    wait();
    delete(netfilterClient);
    delete(internalClient);
    delete(externalClient);
    delete(stateParser);
}

void EventManager::setEventConf(const string &eventHeadText, const string &eventBodyText, const string &eventTailText) {
    this->eventHeadText = eventHeadText;
    this->eventBodyText = eventBodyText;
    this->eventTailText = eventTailText;
}

void EventManager::setNetfilterConf(const string &vmIp, const string &externalIp) {
    this->vmIp = vmIp;
    this->externalIp = externalIp;
}

void EventManager::setSerialPortConf(const string &pseudoTerminal, const string &serialPort) {
    this->pseudoTerminal = pseudoTerminal;
    this->serialPort = serialPort;
}

void EventManager::stop() {
    threadStop = true;
}

void EventManager::run() {
    threadStop = false;

    // 先初始化模型
    stateFile.open(stateFilePath, ifstream::in);
    if (!stateFile.is_open()) {
        emit sendLogMessage(("不存在状态机文件:" + stateFilePath).c_str());
        return;
    }
    string file, line;
    while (!stateFile.eof()) {
        getline(stateFile, line);
        file += line;
    }
    stateParser->setStateXML(file);
    if (!stateParser->parseStateXML()) {
        emit sendLogMessage(("状态机文件 " + stateFilePath + " 无法正常解析").c_str());
    }
    stateFile.close();

    // 再启动netfilterClient
    netfilterClient->setEventMatchText(eventHeadText, eventTailText);
    netfilterClient->setEventMatchIp(vmIp, externalIp);
    if (!netfilterClient->install()) {
        emit sendLogMessage("install netfilter failed!");
        return;
    }
    if (!netfilterClient->start()) {
        emit sendLogMessage("netfilter客户端初始化失败!");
        netfilterClient->stop();
        netfilterClient->remove();
        return;
    }

    // 再启动serialPortClient
    internalClient->setPort(pseudoTerminal);
    externalClient->setPort(serialPort);
    if (!internalClient->init() || !externalClient->init()) {
        emit sendLogMessage("串口转发器初始化失败！");
        internalClient->closePort();
        externalClient->closePort();
        return;
    }

    while (!threadStop) {
        // 轮询各个客户端
        if (netfilterClient->hasEvent()) {
            string event = netfilterClient->getEvent();
            emit sendLogMessage(QString::fromStdString("网络事件：" + event));
        }

        // 判断串口有没有事件
        if (internalClient->hasMessage()) {
            string event = internalClient->getMessage();
            emit sendLogMessage(QString::fromStdString("串口事件，虚拟机-->外部设备：" + event));
            externalClient->sendMessage(event);
        }
        if (externalClient->hasMessage()) {
            string event = externalClient->getMessage();
            emit sendLogMessage(QString::fromStdString("串口事件，外部设备-->虚拟机：" + event));
            internalClient->sendMessage(event);
        }
        // 判断内存有没有事件
    }

    // 先关闭netfilterClient
    netfilterClient->stop();
    netfilterClient->remove();

    // 再关闭serialPortClient
    internalClient->closePort();
    externalClient->closePort();
}

void EventManager::setStateConf(const string &stateFilePath) {
    this->stateFilePath = stateFilePath;
}
