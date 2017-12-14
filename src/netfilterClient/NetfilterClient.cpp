//
// Created by yingzi on 2017/11/7.
//

#include "NetfilterClient.h"

NetfilterClient::NetfilterClient() {
    netlink = new Netlink();
}

NetfilterClient::~NetfilterClient() {
    stop();
    remove();
    delete(netlink);
}

bool NetfilterClient::install() {
    logger->debug("安装netfilter内核模块");
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

const char *NetfilterClient::getEvent() {
    return netlink->getMessage();
}

bool NetfilterClient::passEvent() {
    return netlink->sendAcceptMessage();
}

bool NetfilterClient::interceptEvent() {
    return netlink->sendDropMessage();
}

int NetfilterClient::getFd() {
    return netlink->getFd();
}
