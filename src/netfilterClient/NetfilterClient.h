//
// Created by yingzi on 2017/11/7.
//

#ifndef IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H
#define IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H

#include <QtCore/QThread>
#include <string>
using std::string;

/**
 * 提供对netfilter的配置安装与卸载
 * 从netfilter获取事件
 */
class NetfilterClient : public QThread {
  public:
    explicit NetfilterClient(QObject *parent = nullptr);
    ~NetfilterClient() override ;
    void setEventMatchText(const string &eventHeadText, const string &eventTailText);
    void setEventMatchIp(const string &vmIp, const string &externalIp);
    bool hasEvent();
    void getEvent();
    bool install();
    void remove();
    void stop();

  protected:
    void run() override ;

  private:
    bool threadStop = false;

    QAtomicInt hasEventFlag;

    string eventHeadText;
    string eventTailText;
    string vmIp;
    string externalIp;
};


#endif //IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H
