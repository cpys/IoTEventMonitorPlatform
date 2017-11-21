//
// Created by yingzi on 2017/11/8.
//

#include "EventManager.h"

EventManager::EventManager(QObject *parent) : QThread(parent) {
    netfilterClient = new NetfilterClient();
    serialPortRepeater = new SerialPortRepeater();
    stateParser = new StateParser();
}

EventManager::~EventManager() {
    stop();
    wait();
    delete(netfilterClient);
    delete(serialPortRepeater);
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
    serialPortRepeater->setEventMatchText(eventHeadText, eventTailText);
    serialPortRepeater->setPorts(pseudoTerminal, serialPort);
    if (!serialPortRepeater->init()) {
        emit sendLogMessage("串口转发器初始化失败！");
        serialPortRepeater->closePorts();
        netfilterClient->stop();
        netfilterClient->remove();
        return;
    }

    while (!threadStop) {
        // 轮询各个客户端
        if (netfilterClient->hasEvent()) {
            string event = netfilterClient->getEvent();
            emit sendLogMessage(QString::fromStdString("网络事件：" + event));
            if (stateParser->validateEvent(event)) {
                emit sendLogMessage("验证通过");
                if (!netfilterClient->passEvent()) {
                    emit sendLogMessage("通过指令发送失败！");
                }
            }
            else {
                emit sendLogMessage("验证拦截");
                if (!netfilterClient->interceptEvent()) {
                    emit sendLogMessage("拦截指令发送失败！");
                }
            }
        }

        // 判断串口有没有事件
        if (serialPortRepeater->hasEvent()) {
            string event = serialPortRepeater->getEvent();
            emit sendLogMessage(QString::fromStdString("串口事件：" + event));
            if (stateParser->validateEvent(event)) {
                emit sendLogMessage("验证通过");
                serialPortRepeater->passEvent();
            }
            else {
                emit sendLogMessage("验证拦截");
                serialPortRepeater->interceptEvent();
            }
        }
        // 判断内存有没有事件
    }

    // 先关闭netfilterClient
    netfilterClient->stop();
    netfilterClient->remove();

    // 再关闭serialPortClient
    serialPortRepeater->closePorts();
}

void EventManager::setStateConf(const string &stateFilePath) {
    this->stateFilePath = stateFilePath;
}
