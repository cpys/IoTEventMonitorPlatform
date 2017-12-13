//
// Created by chenkuan on 2017/12/13.
//

#ifndef IOTEVENTMONITORPLATFORM_MEMORYCLIENT_H
#define IOTEVENTMONITORPLATFORM_MEMORYCLIENT_H

#include <string>
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
    int socketFd = -1;
};


#endif //IOTEVENTMONITORPLATFORM_MEMORYCLIENT_H
