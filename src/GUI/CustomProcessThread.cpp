
#include <QtCore/QThread>
#include <iostream>
#include "CustomProcessThread.h"

void CustomProcessThread::stop() {
    threadStop = true;
}

void CustomProcessThread::setHostIp(const string &hostIp) {
    this->hostIp = hostIp;
}

void CustomProcessThread::run() {
    // TODO
    threadStop = false;

    // 先连接host，连接失败直接退出线程
    if (!connectToHost()) {
        sendStatusMessage(("连接宿主机" + hostIp + "失败!").c_str());
        return;
    }
    else {
        sendStatusMessage(("连接宿主机" + hostIp + "成功!").c_str());
    }

    while (!threadStop) {
        sleep(1);
    }
}

bool CustomProcessThread::connectToHost() {
    // TODO
    return true;
}
