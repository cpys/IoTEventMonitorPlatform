//
// Created by yingzi on 2017/11/8.
//

#ifndef IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
#define IOTEVENTMONITORPLATFORM_EVENTMANAGER_H

#include <QtCore/QThread>
#include <string>
#include <fstream>
#include <NetfilterClient.h>
#include <SerialPortClient.h>
#include <SerialPortRepeater.h>
#include <Logger.h>
#include <StateMachineParser.h>

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

    void setEventConf(const QString &eventHead, const QString &eventTail);

    void setNetfilterConf(const QString &vmIp, const QString &externalIp);

    void setSerialPortConf(const QString &pseudoTerminal, const QString &serialPort);

    void setStateMachineConf(const QString &stateFilePath);

    void stop();

  signals:

    void showLogMessage(const QString &);

  protected:
    void run() override ;

  private:
    bool threadStop = false;

    QString eventHead;
    QString eventTail;

    QString vmIp;
    QString externalIp;

    QString pseudoTerminal;
    QString serialPort;

    QString stateMachineFilePath;

    NetfilterClient *netfilterClient = nullptr;
    SerialPortRepeater *serialPortRepeater = nullptr;

    Logger *logger = Logger::getLogger();
};


#endif //IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
