//
// Created by chenkuan on 2017/12/13.
//

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
#include "MemoryClient.h"

MemoryClient::~MemoryClient() {
    stop();
}

const char *MemoryClient::getEvent() {
    bufferOffset = 0;
    while (true) {
        ssize_t recvNum = recv(clientSocket, buffer + bufferOffset, MIN_BUFFER_SIZE, 0);
        if (recvNum < 0) {
            logger->error("接收事件失败！%s", strerror(errno));
        }
        else if (recvNum < MIN_BUFFER_SIZE) {
            bufferOffset += recvNum;
            char *endPos = strchr(buffer, '\0');

            break;
        }
        else {
            bool hasEndChar = false;
            for (int offset = bufferOffset; offset < bufferOffset + MIN_BUFFER_SIZE; ++offset) {
                if (buffer[offset] == '\0') {
                    hasEndChar = true;
                    break;
                }
            }
            if (hasEndChar) {
                return buffer;
            }
            bufferOffset += MIN_BUFFER_SIZE;
        }
    }

    memset(buffer, 0, sizeof(buffer));
    ssize_t recvNum = recv(clientSocket, buffer, MIN_BUFFER_SIZE, 0);
    if (recvNum < 0) {
        logger->error("接收事件失败！%s", strerror(errno));
    }
    else if (recvNum == 0) {
        logger->warning("服务端已经断开连接");
    }
    else {
        logger->info("接收到内存事件数据：%s", buffer);
    }
    return buffer;
}

bool MemoryClient::start() {
    logger->debug("启动内存事件获取客户端");
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        logger->error("创建套接字失败！");
        return false;
    }

    // 配置服务端套接字
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(MEMORY_SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(MEMORY_SERVER_IP);

    // 连接服务端
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        logger->error("连接服务端失败！");
        return false;
    }

    return true;
}

bool MemoryClient::stop() {
    logger->debug("停止内存事件获取客户端");
    if (clientSocket < 0) return true;
    close(clientSocket);
    return true;
}

int MemoryClient::getFd() {
    return clientSocket;
}