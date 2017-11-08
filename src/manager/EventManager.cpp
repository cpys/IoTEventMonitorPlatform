//
// Created by yingzi on 2017/11/8.
//

#include "EventManager.h"

EventManager::EventManager(QObject *parent) : QThread(parent) {
    netfilterClient = new NetfilterClient(this);
    QObject::connect(netfilterClient, SIGNAL(sendLogMessage(const QString&)), this, SLOT(recvLogMessage(const QString&)));
}

EventManager::~EventManager() {
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
    sendLogMessage("start eventManager!");
    threadStop = false;

    // 先启动netfilterClient
    netfilterClient->setEventMatchText(eventHeadText, eventTailText);
    netfilterClient->setEventMatchIp(vmIp, externalIp);
    if (!netfilterClient->install()) {
        sendLogMessage("install netfilter failed!");
        return;
    }
    netfilterClient->start();

    while (!threadStop) {
        // 轮询各个客户端
        if (netfilterClient->hasEvent()) {
            netfilterClient->getEvent();
            // TODO
            continue;
        }
        // 判断串口有没有事件
        // 判断内存有没有事件
    }
    netfilterClient->stop();
    netfilterClient->wait();
    netfilterClient->remove();
    sendLogMessage("end eventManager!");
}

void EventManager::recvLogMessage(const QString &message) {
    emit sendLogMessage(message);
}
