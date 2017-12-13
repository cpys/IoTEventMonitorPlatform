//
// Created by yingzi on 2017/11/8.
//

#include "EventManager.h"

EventManager::EventManager(QObject *parent) : QThread(parent) {
    netfilterClient = new NetfilterClient();
    serialPortRepeater = new SerialPortRepeater();
    stateParser = new StateParser();
}

EventManager::~EventManager() {
    stop();
    wait();
    delete(netfilterClient);
    delete(serialPortRepeater);
    delete(stateParser);
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

void EventManager::setSerialPortConf(const string &pseudoTerminal, const string &serialPort) {
    this->pseudoTerminal = pseudoTerminal;
    this->serialPort = serialPort;
}

void EventManager::stop() {
    threadStop = true;
}

void EventManager::run() {
    threadStop = false;

    // 先初始化模型
    stateFile.open(stateFilePath, ifstream::in);
    if (!stateFile.is_open()) {
        emit sendLogMessage(("不存在状态机文件:" + stateFilePath).c_str());
        logger->error("不存在状态机文件%s", stateFilePath.c_str());
        return;
    }
    string file, line;
    while (!stateFile.eof()) {
        getline(stateFile, line);
        file += line;
    }
    stateParser->setStateXML(file);
    if (!stateParser->parseStateXML()) {
        emit sendLogMessage(("状态机文件 " + stateFilePath + " 无法正常解析").c_str());
        logger->error("状态机文件 %s 无法正常解析", stateFilePath.c_str());
    }
    stateFile.close();

    // 再启动netfilterClient
    netfilterClient->setEventMatchText(eventHeadText, eventTailText);
    netfilterClient->setEventMatchIp(vmIp, externalIp);
    if (!netfilterClient->install()) {
        emit sendLogMessage("安装netfilter失败!");
        logger->error("安装netfilter失败!");
        return;
    }
    if (!netfilterClient->start()) {
        emit sendLogMessage("netfilter客户端初始化失败!");
        logger->error("netfilter客户端初始化失败!");
        netfilterClient->stop();
        netfilterClient->remove();
        return;
    }
    socketNetlink = netfilterClient->getFd();

    // 再启动serialPortClient
    serialPortRepeater->setEventMatchText(eventHeadText, eventTailText);
    serialPortRepeater->setPorts(pseudoTerminal, serialPort);
    if (!serialPortRepeater->init()) {
        emit sendLogMessage("串口转发器初始化失败！");
        logger->error("串口转发器初始化失败！");
        serialPortRepeater->closePorts();
        netfilterClient->stop();
        netfilterClient->remove();
        return;
    }
    fdPseudoTerminal = serialPortRepeater->getPseudoTerminalFd();
    fdSerialPort = serialPortRepeater->getSerialPortFd();

    // 最后启动memoryClient
    if (!memoryCleint->start()) {
        emit sendLogMessage("内存事件获取服务器连接失败！");
        logger->error("内存事件获取服务器连接失败！");
        memoryCleint->stop();
        return;
    }
    socketMemoryClient = memoryCleint->getFd();

    int maxfd = std::max(std::max(socketNetlink, std::max(fdPseudoTerminal, fdSerialPort)), socketMemoryClient);

    uint eventNum = 0;
    uint interceptNum = 0;
    uint interceptFailedNum = 0;
    const char *event = nullptr;
    while (!threadStop) {
        // 轮询各个客户端
        FD_ZERO(&fs_read);
        FD_SET(socketNetlink, &fs_read);
        FD_SET(fdPseudoTerminal, &fs_read);
        FD_SET(fdSerialPort, &fs_read);
        tv = defaultTv;

        if (select(maxfd + 1, &fs_read, NULL, NULL, &tv) > 0) {
            if (FD_ISSET(socketNetlink, &fs_read)) {
                event = netfilterClient->getEvent();
                logger->info("采集到网络事件：%s", event);
                ++eventNum;

//                netfilterClient->passEvent();
//                if (stateParser->justGetIsEventImportant(event)) {
//                    emit sendLogMessage(QString::fromStdString("网络事件：" + event));
//                }
//                netfilterClient->passEvent();

                bool result = stateParser->validateEvent(event);
                if (stateParser->getIsEventImportant()) {
                    emit sendLogMessage(("采集到网络通信关键事件:" + string(event)).c_str());
                    logger->debug("该事件为关键事件");
                    if (result) {
                        emit sendLogMessage("验证事件后通过此事件");
                        logger->info("网络事件 \"%s\" 验证通过", event);
                        if (!netfilterClient->passEvent()) {
                            emit sendLogMessage("通过指令发送失败！");
                            logger->warning("通过指令发送失败！");
                        }
                    }
                    else {
                        emit sendLogMessage("验证事件后拦截此事件");
                        logger->info("网络事件 \"%s\"验证拦截", event);
                        ++interceptNum;
                        if (!netfilterClient->interceptEvent()) {
                            emit sendLogMessage("拦截指令发送失败！");
                            logger->warning("拦截指令发送失败！");
                            ++interceptFailedNum;
                        }
                    }
                }
                else {
                    logger->debug("该事件为非关键事件");
                    if (result) {
                        logger->info("网络事件 \"%s\" 验证通过", event);
                    }
                    else {
                        logger->info("网络事件 \"%s\" 验证不通过", event);
                    }
                }
            }
            else if (FD_ISSET(fdPseudoTerminal, &fs_read) || FD_ISSET(fdSerialPort, &fs_read)) {
                if (FD_ISSET(fdPseudoTerminal, &fs_read)) {
                    event = serialPortRepeater->getEvent(fdPseudoTerminal);
                    logger->info("采集到串口事件(虚拟机-->外部设备)：%s", event);
                }
                else if (FD_ISSET(fdSerialPort, &fs_read)) {
                    event = serialPortRepeater->getEvent(fdSerialPort);
                    logger->info("采集到串口事件(外部设备-->虚拟机)：%s", event);
                }

                bool result = stateParser->validateEvent(event);
                if (stateParser->getIsEventImportant()) {
                    emit sendLogMessage(("采集到串口通信关键事件:" + string(event)).c_str());
                    logger->debug("该事件为关键事件");
                    if (result) {
                        emit sendLogMessage("验证事件后通过此事件");
                        logger->info("串口事件 \"%s\" 验证通过", event);
                        serialPortRepeater->passEvent();
                    }
                    else {
                        emit sendLogMessage("验证事件后拦截此事件");
                        logger->info("串口事件 \"%s\"验证拦截", event);
                        serialPortRepeater->interceptEvent();
                    }
                }
                else {
                    logger->debug("该事件为非关键事件");
                    if (result) {
                        logger->info("串口事件 \"%s\" 验证通过", event);
                    }
                    else {
                        logger->info("串口事件 \"%s\" 验证不通过", event);
                    }
                }
            }
            else if (FD_ISSET(socketMemoryClient, &fs_read)) {
                event = memoryCleint->getEvent();
                logger->info("采集到内存事件：%s", event);

                bool result = stateParser->validateEvent(event);
                if (stateParser->getIsEventImportant()) {
                    emit sendLogMessage(("采集到内存关键事件:" + string(event)).c_str());
                    logger->debug("该事件为关键事件");
                    if (result) {
                        emit sendLogMessage("内存事件验证可通过");
                        logger->info("内存事件 \"%s\" 验证通过", event);
                    }
                    else {
                        emit sendLogMessage("内存事件验证后不通过");
                        logger->warning("内存事件 \"%s\"验证不通过", event);
                    }
                }
                else {
                    logger->debug("该事件为非关键事件");
                    if (result) {
                        logger->info("内存事件 \"%s\" 验证通过", event);
                    }
                    else {
                        logger->info("内存事件 \"%s\" 验证不通过", event);
                    }
                }

            }
        }
    }
    logger->debug("接收到事件总数为%d", eventNum);
    logger->debug("拦截失败的事件/应该拦截的事件为%d/%d", interceptFailedNum, interceptNum);

    // 先关闭netfilterClient
    netfilterClient->stop();
    netfilterClient->remove();

    // 再关闭serialPortClient
    serialPortRepeater->closePorts();

    // 最后关闭memoryClient
    memoryCleint->stop();
}

void EventManager::setStateConf(const string &stateFilePath) {
    this->stateFilePath = stateFilePath;
}
