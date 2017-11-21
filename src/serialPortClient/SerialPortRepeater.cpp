//
// Created by chenkuan on 17-11-21.
//

#include "SerialPortRepeater.h"

SerialPortRepeater::~SerialPortRepeater() {
    if (pseudoTerminal != nullptr) {
        pseudoTerminal->closePort();
    }
    if (serialPort != nullptr) {
        serialPort->closePort();
    }
}

void SerialPortRepeater::setPorts(const string &pseudoTerminalName, const string &serialPortName) {
    this->pseudoTerminalName = pseudoTerminalName;
    this->serialPortName = serialPortName;
}

bool SerialPortRepeater::init() {
    delete(pseudoTerminal);
    delete(serialPort);
    pseudoTerminal = new SerialPortClient();
    serialPort = new SerialPortClient();

    pseudoTerminal->setPort(pseudoTerminalName);
    serialPort->setPort(serialPortName);
    if (!pseudoTerminal->init() || !serialPort->init()) {
        return false;
    }
    return true;
}

void SerialPortRepeater::closePorts() {
    pseudoTerminal->closePort();
    serialPort->closePort();
}

bool SerialPortRepeater::hasEvent() {
    lastEvent = "";
    if (pseudoTerminal->hasMessage()) {
        pseudoTerminal->getMessage();
        if (searchEvent(pseudoTerminal->getMessageQueue(), serialPort)){
            lastDevice = pseudoTerminal;
            return true;
        }
    }
    if (serialPort->hasMessage()) {
        serialPort->getMessage();
        if (searchEvent(serialPort->getMessageQueue(), pseudoTerminal)) {
            lastDevice = serialPort;
            return true;
        }
    }
    return false;
}

const string &SerialPortRepeater::getEvent() {
    return lastEvent;
}

void SerialPortRepeater::sendEvent(const string &event) {
    if (lastDevice == pseudoTerminal) {
        serialPort->sendMessage(event);
    }
    else if (lastDevice == serialPort) {
        pseudoTerminal->sendMessage(event);
    }
}

void SerialPortRepeater::setEventMatchText(const string &eventHeadText, const string &eventTailText) {
    this->eventHeadText = eventHeadText;
    this->eventTailText = eventTailText;
}

bool SerialPortRepeater::searchEvent(string &messageQueue, SerialPortClient *otherPort) {
    auto headIndex = messageQueue.find(eventHeadText);
    if (headIndex != string::npos) {
        auto tailIndex = messageQueue.find(eventTailText);
        if (tailIndex != string::npos) {
            otherPort->sendMessage(messageQueue.substr(0, headIndex));
            lastEvent = messageQueue.substr(headIndex, tailIndex - headIndex + eventTailText.size());
            messageQueue = messageQueue.substr(tailIndex + eventTailText.size());
            return true;
        }
        else if (messageQueue.size() >= MAX_MESSAGE_SIZE){
            // 如果尾巴没找到但超长了，视为非事件转发
            otherPort->sendMessage(messageQueue);
            messageQueue = "";
        }
    }
    else {
        // 如果头部没找到，视为非事件转发，注意保留队列头部长度 - 1个字节
        if (messageQueue.size() >= eventHeadText.size()) {
            otherPort->sendMessage(messageQueue.substr(0, messageQueue.size() - eventHeadText.size() + 1));
            messageQueue = messageQueue.substr(messageQueue.size() - eventHeadText.size() + 1);
        }
        else {
            otherPort->sendMessage(messageQueue.substr(0, 1));
            messageQueue = messageQueue.substr(1);
        }
    }
    return false;
}

void SerialPortRepeater::passEvent() {
    sendEvent(lastEvent);
}

void SerialPortRepeater::interceptEvent() {
    // pass
}
