//
// Created by yingzi on 2017/11/7.
//

#include "NetfilterClient.h"

NetfilterClient::NetfilterClient(QObject *parent) : QThread(parent) {
    netlink = new Netlink();
}

NetfilterClient::~NetfilterClient() {
    stop();
    wait();
    remove();
    delete(netlink);
}

bool NetfilterClient::install() {
//    emit sendLogMessage("install netfilter!");
    return true;
}

void NetfilterClient::remove() {
//    emit sendLogMessage("remove netfilter!");
}

void NetfilterClient::run() {
    threadStop = false;
    hasEventFlag = false;

    if (!netlink->init()) {
        emit sendLogMessage("init netlink failed!");
    }
    else {
        while (!threadStop) {
//        emit sendLogMessage("netfilter client is running!");
//        sleep(1);
            event = netlink->getMessage();
            hasEventFlag = true;
        }
    }
    netlink->closeConnection();
    emit sendLogMessage("netfilter client is stop!");
}

void NetfilterClient::stop() {
    threadStop = true;
}

void NetfilterClient::setEventMatchText(const string &eventHeadText, const string &eventTailText) {
    this->eventHeadText = eventHeadText;
    this->eventTailText = eventTailText;
}

void NetfilterClient::setEventMatchIp(const string &vmIp, const string &externalIp) {
    this->vmIp = vmIp;
    this->externalIp = externalIp;
}

bool NetfilterClient::hasEvent() {
    return hasEventFlag;
}

string NetfilterClient::getEvent() {
    hasEventFlag = false;
    return event;
}
