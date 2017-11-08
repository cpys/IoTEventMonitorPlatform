//
// Created by yingzi on 2017/11/8.
//

#include <iostream>
#include "EventManager.h"
using std::cout;
using std::endl;

EventManager::EventManager(QObject *parent) : QThread(parent) {
    netfilterClient = new NetfilterClient(this);
}

EventManager::~EventManager() {
    cout << "deconstruct eventmanager!" << endl;
    stop();
    wait();
    delete(netfilterClient);
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

void EventManager::stop() {
    threadStop = true;
}

void EventManager::run() {
    threadStop = false;

    // 先启动netfilterClient
    netfilterClient->setEventMatchText(eventHeadText, eventTailText);
    netfilterClient->setEventMatchIp(vmIp, externalIp);
    if (!netfilterClient->install()) {
        sendStatusMessage("安装netfilter失败！");
        return;
    }
    netfilterClient->start();

    while (!threadStop) {
        // 轮询各个客户端
        if (netfilterClient->hasEvent()) {
            netfilterClient->getEvent();
            // TODO deal eventlist
            continue;
        }
        // 判断串口有没有事件
        // 判断内存有没有事件
    }
    netfilterClient->stop();
    netfilterClient->wait();
}