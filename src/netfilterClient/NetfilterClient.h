//
// Created by yingzi on 2017/11/7.
//

#ifndef IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H
#define IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H

#include <QtCore/QThread>
#include <string>
#include "Netlink.h"

using std::string;

/**
 * 提供对netfilter的配置安装与卸载
 * 从netfilter获取事件
 */
class NetfilterClient : QObject {
  Q_OBJECT

  public:
    explicit NetfilterClient(QObject *parent = nullptr);
    ~NetfilterClient() ;
    void setEventMatchText(const string &eventHeadText, const string &eventTailText);
    void setEventMatchIp(const string &vmIp, const string &externalIp);
    bool hasEvent();
    string getEvent();
    bool install();
    void remove();
    void start();
    void stop();

  signals:
    void sendLogMessage(const QString&);

//  protected:
//    void run() override ;

  private:
    bool threadStop = false;

    bool hasEventFlag;

    string eventHeadText;
    string eventTailText;
    string vmIp;
    string externalIp;

    Netlink *netlink;
    string event;
};


#endif //IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H
