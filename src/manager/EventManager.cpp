//
// Created by yingzi on 2017/11/8.
//

#include "EventManager.h"

EventManager::EventManager() {
    netfilterClient = new NetfilterClient();
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
    threadStop = false;

    while (!threadStop) {
        sleep(2);
    }
}