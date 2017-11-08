//
// Created by yingzi on 2017/11/7.
//

#include "NetfilterClient.h"

NetfilterClient::NetfilterClient(QObject *parent) : QThread(parent) {

}

NetfilterClient::~NetfilterClient() {
    stop();
    wait();
    remove();
}

bool NetfilterClient::install() {
    emit sendLogMessage("install netfilter!");
    return true;
}

void NetfilterClient::remove() {
    emit sendLogMessage("remove netfilter!");
}

void NetfilterClient::run() {
    threadStop = false;

    while (!threadStop) {
        emit sendLogMessage("netfilter client is running!");
        sleep(1);
    }
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
    return false;
}

void NetfilterClient::getEvent() {

}
