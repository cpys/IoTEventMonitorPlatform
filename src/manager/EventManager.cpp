//
// Created by yingzi on 2017/11/8.
//

#include <iostream>
#include "EventManager.h"
using namespace std;

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

    //uint eventNum = 0;
    while (!threadStop) {
        // 轮询各个客户端
//        cout << "netfilter client has event?" << endl;
        if (netfilterClient->hasEvent()) {
//            cout << "netfilter client has event, let's get event" << endl;
            string event = netfilterClient->getEvent();
            cout << "采集到网络事件：" << event << endl;
//            cout << "get netfilter event " << event << endl;
            //cout << "采集到网络事件 " << event << ",事件总数为" << ++eventNum << endl;
            //++eventNum;
            //usleep(1500);
            //netfilterClient->passEvent();
//            if (stateParser->justGetIsEventImportant(event)) {
//                emit sendLogMessage(QString::fromStdString("网络事件：" + event));
//            }
//            netfilterClient->passEvent();
            bool result = stateParser->validateEvent(event);
            if (stateParser->getIsEventImportant()) {
                emit sendLogMessage(("采集到网络通信关键事件:" + event).c_str());
                cout << "  该事件为关键事件" << endl;
                if (result) {
                    emit sendLogMessage("验证事件后通过此事件");
                    cout << "  该事件验证通过" << endl;
                    if (!netfilterClient->passEvent()) {
                        emit sendLogMessage("通过指令发送失败！");
                        cerr << "  通过指令发送失败！" << endl;
                    }
                }
                else {
                    emit sendLogMessage("验证事件后拦截此事件");
                    cout << "  该事件验证拦截" << endl;
                    if (!netfilterClient->interceptEvent()) {
                        emit sendLogMessage("拦截指令发送失败！");
                        cerr << "  拦截指令发送失败！" << endl;
                    }
                }
            }
            else {
                cout << "  该事件为非关键事件" << endl;
                if (result) {
                    cout << "  该事件验证通过" << endl;
                }
                else {
                    cout << "  该事件验证未通过" << endl;
                }
            }
        }

        // 判断串口有没有事件
        if (serialPortRepeater->hasEvent()) {
            string event = serialPortRepeater->getEvent();
            cout << "采集到串口事件：" << event << endl;

            bool result = stateParser->validateEvent(event);
            if (stateParser->getIsEventImportant()) {
                emit sendLogMessage(("采集到串口通信关键事件:" + event).c_str());
                cout << "  该事件为关键事件" << endl;
                if (result) {
                    emit sendLogMessage("验证事件后通过此事件");
                    cout << "  该事件验证通过" << endl;
                    serialPortRepeater->passEvent();
                }
                else {
                    emit sendLogMessage("验证事件后拦截此事件");
                    cout << "  该事件验证拦截" << endl;
                    serialPortRepeater->interceptEvent();
                }
            }
            else {
                cout << "  该事件为非关键事件" << endl;
                if (result) {
                    cout << "  该事件验证通过" << endl;
                }
                else {
                    cout << "  该事件验证未通过" << endl;
                }
            }

/*
            emit sendLogMessage(QString::fromStdString("串口事件：" + event));
            cout << "采集到串口事件" << event << ",事件总数为" << ++eventNum << endl;
            if (stateParser->validateEvent(event)) {
                emit sendLogMessage("验证通过");
                serialPortRepeater->passEvent();
            }
            else if (stateParser->getIsEventImportant()){
                // 只有关键事件才进行拦截
                emit sendLogMessage("验证拦截");
                serialPortRepeater->interceptEvent();
            }
            else {
                // 非关键事件只进行报警
                emit sendLogMessage("事件验证失败");
                serialPortRepeater->passEvent();
            }
*/
        }
        // 判断内存有没有事件
    }
    //cout << "接收到事件总数为" << eventNum << endl;

    // 先关闭netfilterClient
    netfilterClient->stop();
    netfilterClient->remove();

    // 再关闭serialPortClient
    serialPortRepeater->closePorts();
}

void EventManager::setStateConf(const string &stateFilePath) {
    this->stateFilePath = stateFilePath;
}
