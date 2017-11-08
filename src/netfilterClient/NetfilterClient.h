//
// Created by yingzi on 2017/11/7.
//

#ifndef IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H
#define IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H

#include <QtCore/QThread>

/**
 * 提供对netfilter的配置安装与卸载
 * 从netfilter获取事件
 */
class NetfilterClient : public QThread {
  public:
    bool install();
    void remove();

  protected:
    void run() override ;
};


#endif //IOTEVENTMONITORPLATFORM_NETFILTERCLIENT_H
