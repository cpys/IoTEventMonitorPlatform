//
// Created by chenkuan on 2017/12/13.
//

#include "MemoryClient.h"

MemoryClient::~MemoryClient() {
    close(socketFd);
}

const char *MemoryClient::getEvent() {
    return nullptr;
}

bool MemoryClient::start() {
    return true;
}

bool MemoryClient::stop() {
    return true;
}

bool MemoryClient::getFd() {
    return socketFd;
}