//
// Created by yingzi on 2017/11/8.
//

#include "EventManager.h"

EventManager::EventManager(QObject *parent) : QThread(parent) {
    netfilterClient = new NetfilterClient();
    serialPortRepeater = new SerialPortRepeater();
    memoryCleint = new MemoryClient();
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
    emit sendLogMessage("初始化状态机...");
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
    stateFile.close();

    stateParser->setStateXML(file);
    if (!stateParser->parseStateXML()) {
        emit sendLogMessage(("状态机文件 " + stateFilePath + " 无法正常解析").c_str());
        logger->error("状态机文件 %s 无法正常解析", stateFilePath.c_str());
        return;
    }
    emit sendLogMessage("状态机初始化完成！");

    bool hasNetfilterClient = true, hasSerialPortRepeater = true, hasMemoryClient = true;

    // 再启动netfilterClient
    netfilterClient->setEventMatchText(eventHeadText, eventTailText);
    netfilterClient->setEventMatchIp(vmIp, externalIp);

    emit sendLogMessage("初始化netfilter...");
    if (!netfilterClient->install()) {
        emit sendLogMessage("安装netfilter失败!");
        logger->error("安装netfilter失败!");
        hasNetfilterClient = false;
    }
    else {
        emit sendLogMessage("成功编译安装netfilter内核模块！");
    }

    if (hasNetfilterClient) {
        emit sendLogMessage("连接内核中的netfilter模块...");
        if (!netfilterClient->start()) {
            emit sendLogMessage("netfilter连接失败!");
            logger->error("netfilter连接失败!");
            netfilterClient->stop();
            netfilterClient->remove();
            hasNetfilterClient = false;
        }
        else {
            emit sendLogMessage("成功连接到内核中的netfilter模块！");
        }
    }
    socketNetlink = netfilterClient->getFd();

    // 再启动serialPortClient
    serialPortRepeater->setEventMatchText(eventHeadText, eventTailText);
    serialPortRepeater->setPorts(pseudoTerminal, serialPort);

    emit sendLogMessage("初始化串口转发器...");
    if (!serialPortRepeater->init()) {
        emit sendLogMessage("串口转发器初始化失败！");
        logger->error("串口转发器初始化失败！");
        serialPortRepeater->closePorts();
        netfilterClient->stop();
        netfilterClient->remove();
        hasSerialPortRepeater = false;
    }
    else {
        emit sendLogMessage("初始化串口转发器成功！");
    }
    fdPseudoTerminal = serialPortRepeater->getPseudoTerminalFd();
    fdSerialPort = serialPortRepeater->getSerialPortFd();

    // 最后启动memoryClient
    emit sendLogMessage("连接内存事件获取服务器...");
    if (!memoryCleint->start()) {
        emit sendLogMessage("内存事件获取服务器连接失败！");
        logger->error("内存事件获取服务器连接失败！");
        memoryCleint->stop();
        hasMemoryClient = false;
    }
    else {
        emit sendLogMessage("成功连接至内存事件获取服务器！");
    }
    socketMemoryClient = memoryCleint->getFd();

    int maxfd = 0;
    if (hasNetfilterClient) {
        maxfd = socketNetlink;
    }
    if (hasSerialPortRepeater) {
        maxfd = std::max(maxfd, std::max(fdPseudoTerminal, fdSerialPort));
    }
    if (hasMemoryClient) {
        maxfd = std::max(maxfd, socketMemoryClient);
    }
    if (maxfd == 0) {
        return;
    }

    uint eventNum = 0;
    uint interceptNum = 0;
    uint interceptFailedNum = 0;
    const char *event = nullptr;

    while (!threadStop) {
        // 轮询各个客户端
        FD_ZERO(&fs_read);
        if (hasNetfilterClient) {
            FD_SET(socketNetlink, &fs_read);
        }
        if (hasSerialPortRepeater) {
            FD_SET(fdPseudoTerminal, &fs_read);
            FD_SET(fdSerialPort, &fs_read);
        }
        if (hasMemoryClient) {
            FD_SET(socketMemoryClient, &fs_read);
        }
        tv = defaultTv;

        if (select(maxfd + 1, &fs_read, nullptr, nullptr, &tv) > 0) {
            startSerial:
            if (FD_ISSET(fdPseudoTerminal, &fs_read) || FD_ISSET(fdSerialPort, &fs_read)) {
                logger->debug("串口上有数据");
                if (FD_ISSET(fdPseudoTerminal, &fs_read)) {
                    logger->debug("伪终端上有数据");
                    event = serialPortRepeater->getEvent(fdPseudoTerminal);
                    if (event == nullptr) {
                        goto startMemory;
                    }
                    logger->info("采集到串口事件(虚拟机-->外部设备)：%s", event);
                    ++eventNum;
                }
                else if (FD_ISSET(fdSerialPort, &fs_read)) {
                    logger->debug("物理串口上有数据");
                    event = serialPortRepeater->getEvent(fdSerialPort);
                    if (event == nullptr) {
                        goto startMemory;
                    }
                    logger->info("采集到串口事件(外部设备-->虚拟机)：%s", event);
                    ++eventNum;
                }

                enum VerificationResult result = stateParser->validateEvent(event);
                if (stateParser->getIsEventImportant()) {
                    emit sendLogMessage(("采集到串口通信关键事件:" + string(event)).c_str());
                    logger->debug("该事件为关键事件");
                    if (result == ACCEPT) {
                        emit sendLogMessage("验证事件后通过此事件");
                        logger->info("串口事件 \"%s\" 验证通过", event);
                        serialPortRepeater->passEvent();
                    }
                    else if (result == DROP){
                        emit sendLogMessage("验证事件后拦截此事件");
                        logger->info("串口事件 \"%s\"验证拦截", event);
                        ++interceptNum;
                        serialPortRepeater->interceptEvent();
                    }
                    else if (result == PENDING){
                        // TODO
                        // 开始等待后续事件处理结果
                    }
                }
                else {
                    logger->debug("该事件为非关键事件");
                    if (result == ACCEPT || result == PENDING) {
                        logger->info("串口事件 \"%s\" 验证通过", event);
                    }
                    else {
                        logger->info("串口事件 \"%s\" 验证不通过", event);
                    }
                }
            }
            startMemory:
            if (FD_ISSET(socketMemoryClient, &fs_read)) {
                logger->debug("内存服务器上有数据");
                event = memoryCleint->getEvent();
                if (event == nullptr) {
                    goto startNet;
                }
                logger->info("采集到内存事件：%s", event);
                ++eventNum;

                enum VerificationResult result = stateParser->validateEvent(event);
                if (stateParser->getIsEventImportant()) {
                    emit sendLogMessage(("采集到内存关键事件:" + string(event)).c_str());
                    logger->debug("该事件为关键事件");
                    if (result == ACCEPT || result == PENDING) {
                        emit sendLogMessage("内存事件验证可通过");
                        logger->info("内存事件 \"%s\" 验证通过", event);
                    }
                    else if (result == DROP){
                        emit sendLogMessage("内存事件验证后不通过");
                        logger->warning("内存事件 \"%s\"验证不通过", event);
                    }
                }
                else {
                    logger->debug("该事件为非关键事件");
                    if (result == ACCEPT || result == PENDING) {
                        logger->info("内存事件 \"%s\" 验证通过", event);
                    }
                    else {
                        logger->info("内存事件 \"%s\" 验证不通过", event);
                    }
                }
            }
            startNet:
            if (FD_ISSET(socketNetlink, &fs_read)) {
                logger->debug("netlink上有数据");
                event = netfilterClient->getEvent();
                if (event == nullptr) {
                    continue;
                }
                logger->info("采集到网络事件：%s", event);
                ++eventNum;

                enum VerificationResult result = stateParser->validateEvent(event);
                if (stateParser->getIsEventImportant()) {
                    emit sendLogMessage(("采集到网络通信关键事件:" + string(event)).c_str());
                    logger->debug("该事件为关键事件");
                    if (result == ACCEPT) {
                        emit sendLogMessage("验证事件后通过此事件");
                        logger->info("网络事件 \"%s\" 验证通过", event);
                        if (!netfilterClient->passEvent()) {
                            emit sendLogMessage("通过指令发送失败！");
                            logger->warning("通过指令发送失败！");
                        }
                    }
                    else if (result == DROP) {
                        emit sendLogMessage("验证事件后拦截此事件");
                        logger->info("网络事件 \"%s\"验证拦截", event);
                        ++interceptNum;
                        if (!netfilterClient->interceptEvent()) {
                            emit sendLogMessage("拦截指令发送失败！");
                            logger->warning("拦截指令发送失败！");
                            ++interceptFailedNum;
                        }
                    }
                    else if (result == PENDING) {
                        // TODO
                        // 开始等待后续事件结果
                    }
                }
                else {
                    logger->debug("该事件为非关键事件");
                    if (result == ACCEPT || result == PENDING) {
                        logger->info("网络事件 \"%s\" 验证通过", event);
                    }
                    else {
                        logger->info("网络事件 \"%s\" 验证不通过", event);
                    }
                }
            }

        }
    }
    logger->info("接收到事件总数为%d", eventNum);
    logger->info("拦截失败的事件/应该拦截的事件为%d/%d", interceptFailedNum, interceptNum);

    if (hasNetfilterClient) {
        // 先关闭netfilterClient
        netfilterClient->stop();
        netfilterClient->remove();
    }

    if (hasSerialPortRepeater) {
        // 再关闭serialPortClient
        serialPortRepeater->closePorts();
    }

    if (hasMemoryClient) {
        // 最后关闭memoryClient
        memoryCleint->stop();
    }
}

void EventManager::setStateConf(const string &stateFilePath) {
    this->stateFilePath = stateFilePath;
}
