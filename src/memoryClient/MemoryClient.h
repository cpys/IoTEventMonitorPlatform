//
// Created by chenkuan on 2017/12/13.
//

#ifndef IOTEVENTMONITORPLATFORM_MEMORYCLIENT_H
#define IOTEVENTMONITORPLATFORM_MEMORYCLIENT_H

#include <netinet/in.h>
#include <string>
#include <Logger.h>
#include "memoryClientConf.h"

using std::string;

class MemoryClient {
  public:
    MemoryClient() = default;
    ~MemoryClient();
    const char *getEvent();
    bool start();
    bool stop();
    int getFd();

  private:
    int clientSocket = -1;
    struct sockaddr_in serverAddr;

    char buffer[MAX_BUFFER_SIZE];

    Logger *logger = Logger::getLogger();
};


#endif //IOTEVENTMONITORPLATFORM_MEMORYCLIENT_H
