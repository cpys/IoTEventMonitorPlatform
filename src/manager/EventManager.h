//
// Created by yingzi on 2017/11/8.
//

#ifndef IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
#define IOTEVENTMONITORPLATFORM_EVENTMANAGER_H

#include <QtCore/QThread>
#include <NetfilterClient.h>
#include <string>
#include <fstream>
#include <SerialPortClient.h>
#include <SerialPortRepeater.h>
#include <Logger.h>
#include "StateParser.h"

using std::string;
using std::ifstream;

/**
 * 1. 管理各个事件获取客户端
 * 2. 协调从各个客户端获取的事件
 * 3. 对事件进行验证
 * 4. 对日志进行信号回显
 */
class EventManager : public QThread {
  Q_OBJECT

  public:
    explicit EventManager(QObject *parent = nullptr);
    ~EventManager() override ;
    void setEventConf(const string &eventHeadText, const string &eventBodyText, const string &eventTailText);
    void setNetfilterConf(const string &vmIp, const string &externalIp);
    void setSerialPortConf(const string &pseudoTerminal, const string &serialPort);
    void setStateConf(const string &stateFilePath);

    void stop();

  signals:
    void sendStatusMessage(const QString&);
    void sendLogMessage(const QString&);

  protected:
    void run() override ;

  private:
    bool threadStop = false;

    string eventHeadText;
    string eventBodyText;
    string eventTailText;

    string vmIp;
    string externalIp;
    NetfilterClient *netfilterClient = nullptr;

    string pseudoTerminal;
    string serialPort;
    SerialPortRepeater *serialPortRepeater;

    string stateFilePath;
    ifstream stateFile;

    StateParser *stateParser;

    /**
    * 用于select的客户端套接字集合
    */
    fd_set fs_read;
    /**
     * 等待超时时间
     */
    static constexpr timeval defaultTv = {0, 1000};
    timeval tv = defaultTv;

    /**
     * 各个来源客户端的套接字
     */
    int socketNetlink = -1;
    int fdPseudoTerminal = -1;
    int fdSerialPort = -1;

    Logger *logger = Logger::getLogger();
};


#endif //IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
