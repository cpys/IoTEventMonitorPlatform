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
};


#endif //IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
