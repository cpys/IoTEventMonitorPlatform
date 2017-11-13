//
// Created by chenkuan on 17-11-13.
//

#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "Netlink.h"
using std::cout;
using std::endl;

Netlink::~Netlink() {
    cout << "deconstruct netlink!" << endl;
    closeConnection();
}

bool Netlink::init() {
    // 创建客户端原始套接字，协议为NETLINK_TEST
    if ((socketClient = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST)) < 0) {
        return false;
    }

    // 创建本地地址数据结构
    struct sockaddr_nl local;
    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_pid = getpid();
    local.nl_groups = 0;

    // 将套接字与本地地址绑定
    if (bind(socketClient, (struct sockaddr*)&local, sizeof(local)) != 0){
        return false;
    }

    // 设置目标地址数据结构
    memset(&destAddr, 0, sizeof(destAddr));   //清空
    destAddr.nl_family = AF_NETLINK;   // 设置协议簇
    destAddr.nl_pid = 0;   // 接收或发送消息的进程的id，内核处理消息所以设置为0
    destAddr.nl_groups = 0;    // 指定多播组，设置为0表示调用者不加入任何多播组

    // 设置待发送的netlink消息
    struct nlmsghdr message;
    memset(&message, 0, sizeof(message));   // 清空该消息
    message.nlmsg_len = NLMSG_LENGTH(0);    // netlink消息的总长度,仅需提供发送数据的长度，由该宏自动计算对齐后的总长度
    message.nlmsg_flags = 0;   // 用户应用内部定义消息的类型，大部分情况下设置为0
    message.nlmsg_type = NETLINK_TEST_CONNECT;   // 设置消息标志，这里设置为连接时标志
    message.nlmsg_pid = getpid();   // 设置发送者的PID

    // 发送netlink消息到内核
    if (sendto(socketClient, &message, message.nlmsg_len, 0, (struct sockaddr *) &destAddr, sizeof(destAddr)) < 0) {
        printf("send connect message to kernel failed!\n");
        return false;
    }
    else{
        printf("send connect message to kernel success!\n");
    }
    return true;
}

void Netlink::closeConnection() {
    close(socketClient);
}

string Netlink::getMessage()  {
    static int destAddrLen = sizeof(struct sockaddr_nl);
    if (recvfrom(socketClient, &recvMessage, sizeof(recvMessage), 0, (struct sockaddr *) &destAddr, (socklen_t*)&destAddrLen) < 0) {
        printf("recv message from kernel failed!\n");
        return "";
    } else {
        return string(recvMessage.data);
    }
}
