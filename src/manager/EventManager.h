//
// Created by yingzi on 2017/11/8.
//

#ifndef IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
#define IOTEVENTMONITORPLATFORM_EVENTMANAGER_H

#include <QtCore/QThread>
#include <NetfilterClient.h>
#include <string>
using std::string;

/**
 * 1. 管理各个事件获取客户端
 * 2. 协调从各个客户端获取的事件
 * 3. 对事件进行验证
 * 4. 对日志进行信号回显
 */
class EventManager : public QThread {
  Q_OBJECT

  public:
    EventManager();
    ~EventManager();
    void setEventConf(const string &eventHeadText, const string &eventBodyText, const string &eventTailText);
    void setNetfilterConf(const string &vmIp, const string &externalIp);

    void stop();

  signals:
    void sendStatusMessage(const QString&);

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
};


#endif //IOTEVENTMONITORPLATFORM_EVENTMANAGER_H
