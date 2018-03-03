//
// Created by yingzi on 2017/11/8.
//

#include <EventManager.h>
#include <moc_EventManager.cpp>

EventManager::EventManager(QObject *parent) : QThread(parent) {
//    netfilterClient = new NetfilterClient();
//    serialPortRepeater = new SerialPortRepeater();
}

EventManager::~EventManager() {
    stop();
    wait();
//    delete(netfilterClient);
//    delete(serialPortRepeater);
}

void EventManager::setEventConf(const QString &eventHead, const QString &eventTail) {
    this->eventHead = eventHead;
    this->eventTail = eventTail;
}

void EventManager::setNetfilterConf(const QString &vmIp, const QString &externalIp) {
    this->vmIp = vmIp;
    this->externalIp = externalIp;
}

void EventManager::setSerialPortConf(const QString &pseudoTerminal, const QString &serialPort) {
    this->pseudoTerminal = pseudoTerminal;
    this->serialPort = serialPort;
}

void EventManager::setStateMachineConf(const QString &stateMachineFilePath) {
    this->stateMachineFilePath = stateMachineFilePath;
}


void EventManager::stop() {
    threadStop = true;
}

void EventManager::run() {
    threadStop = false;

    while (!threadStop) {
        logger->info("后台线程运行中...");
        sleep(3);
    }

    // 先初始化模型
    Model *model = StateMachineParser::initFromFile(stateMachineFilePath.toLocal8Bit().data());
    if (model == nullptr) {
        logger->error("从文件\"%s\"初始化模型失败！", stateMachineFilePath.toLocal8Bit().data());
        return;
    }


//    // 再启动netfilterClient
//    netfilterClient->setEventMatchText(eventHead, eventTail);
//    netfilterClient->setEventMatchIp(vmIp, externalIp);
//    if (!netfilterClient->install()) {
//        logger->error("安装netfilter失败!");
//        return;
//    }
//    if (!netfilterClient->start()) {
//        logger->error("netfilter客户端初始化失败!");
//        netfilterClient->stop();
//        netfilterClient->remove();
//        return;
//    }
//
//    // 再启动serialPortClient
//    serialPortRepeater->setEventMatchText(eventHead, eventTail);
//    serialPortRepeater->setPorts(pseudoTerminal, serialPort);
//    if (!serialPortRepeater->init()) {
//        logger->error("串口转发器初始化失败！");
//        serialPortRepeater->closePorts();
//        netfilterClient->stop();
//        netfilterClient->remove();
//        return;
//    }
//
//    uint eventNum = 0;
//    uint interceptNum = 0;
//    uint interceptFailedNum = 0;
//    while (!threadStop) {
//        // 轮询各个客户端
//        if (netfilterClient->hasEvent()) {
//            string event = netfilterClient->getEvent();
//            logger->info("采集到网络事件：%s", event.c_str());
//            ++eventNum;
//
//            //netfilterClient->passEvent();
////            if (stateParser->justGetIsEventImportant(event)) {
////                emit sendLogMessage(QString::fromStdString("网络事件：" + event));
////            }
////            netfilterClient->passEvent();
//
//            bool result = stateParser->validateEvent(event);
//            if (stateParser->getIsEventImportant()) {
//                emit sendLogMessage(("采集到网络通信关键事件:" + event).c_str());
//                logger->debug("该事件为关键事件");
//                if (result) {
//                    emit sendLogMessage("验证事件后通过此事件");
//                    logger->info("网络事件 \"%s\" 验证通过", event.c_str());
//                    if (!netfilterClient->passEvent()) {
//                        emit sendLogMessage("通过指令发送失败！");
//                        logger->warning("通过指令发送失败！");
//                    }
//                }
//                else {
//                    emit sendLogMessage("验证事件后拦截此事件");
//                    logger->info("网络事件 \"%s\"验证拦截", event.c_str());
//                    ++interceptNum;
//                    if (!netfilterClient->interceptEvent()) {
//                        emit sendLogMessage("拦截指令发送失败！");
//                        logger->warning("拦截指令发送失败！");
//                        ++interceptFailedNum;
//                    }
//                }
//            }
//            else {
//                logger->debug("该事件为非关键事件");
//                if (result) {
//                    logger->info("网络事件 \"%s\" 验证通过", event.c_str());
//                }
//                else {
//                    logger->info("网络事件 \"%s\" 验证通过", event.c_str());
//                }
//            }
//        }
//
//        // 判断串口有没有事件
//        if (serialPortRepeater->hasEvent()) {
//            string event = serialPortRepeater->getEvent();
//            logger->info("采集到串口事件：%s", event.c_str());
//
//            bool result = stateParser->validateEvent(event);
//            if (stateParser->getIsEventImportant()) {
//                emit sendLogMessage(("采集到串口通信关键事件:" + event).c_str());
//                logger->debug("该事件为关键事件");
//                if (result) {
//                    emit sendLogMessage("验证事件后通过此事件");
//                    logger->info("串口事件 \"%s\" 验证通过", event.c_str());
//                    serialPortRepeater->passEvent();
//                }
//                else {
//                    emit sendLogMessage("验证事件后拦截此事件");
//                    logger->info("串口事件 \"%s\"验证拦截", event.c_str());
//                    serialPortRepeater->interceptEvent();
//                }
//            }
//            else {
//                logger->debug("该事件为非关键事件");
//                if (result) {
//                    logger->info("串口事件 \"%s\" 验证通过", event.c_str());
//                }
//                else {
//                    logger->info("串口事件 \"%s\" 验证通过", event.c_str());
//                }
//            }
//        }
//        // 判断内存有没有事件
//    }
//    logger->debug("接收到事件总数为%d", eventNum);
//    logger->debug("拦截失败的事件/应该拦截的事件为%d/%d", interceptFailedNum, interceptNum);
//
//    // 先关闭netfilterClient
//    netfilterClient->stop();
//    netfilterClient->remove();
//
//    // 再关闭serialPortClient
//    serialPortRepeater->closePorts();
}
