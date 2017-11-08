//
// Created by yingzi on 2017/11/7.
//

#include <iostream>
#include "NetfilterClient.h"
using std::cout;
using std::endl;

NetfilterClient::NetfilterClient(QObject *parent) : QThread(parent) {

}

NetfilterClient::~NetfilterClient() {
    cout << "deconstruct netfilter client!" << endl;
    stop();
    wait();
    remove();
}

bool NetfilterClient::install() {
    cout << "install netfilter!" << endl;
    return true;
}

void NetfilterClient::remove() {
    cout << "remove netfilter!" << endl;
}

void NetfilterClient::run() {
    threadStop = false;

    while (!threadStop) {
        cout << "netfilter client is running!" << endl;
        sleep(1);
    }
    cout << "netfilter client is stop!" << endl;
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
