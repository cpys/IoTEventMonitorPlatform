//
// Created by yingzi on 2017/11/7.
//

#include <iostream>
#include "NetfilterClient.h"
using namespace std;

NetfilterClient::NetfilterClient() {
    netlink = new Netlink();
}

NetfilterClient::~NetfilterClient() {
    stop();
    remove();
    delete(netlink);
}

bool NetfilterClient::install() {
    return true;
}

void NetfilterClient::remove() {

}

bool NetfilterClient::start() {
    return netlink->init();
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
