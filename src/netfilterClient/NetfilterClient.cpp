//
// Created by yingzi on 2017/11/7.
//

#include <iostream>
#include "NetfilterClient.h"
using namespace std;

NetfilterClient::NetfilterClient(QObject *parent) : QObject(parent) {
    netlink = new Netlink();
}

NetfilterClient::~NetfilterClient() {
    stop();
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

void NetfilterClient::start() {
    if (!netlink->init()) {
        emit sendLogMessage("初始化netlink失败！");
    }
    else {
        emit sendLogMessage("初始化netlink成功!");
    }
}

void NetfilterClient::stop() {
    netlink->closeConnection();
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
    return netlink->hasMessage();
}

string NetfilterClient::getEvent() {
    return netlink->getMessage();
}
