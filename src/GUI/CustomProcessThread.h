//
// Created by yingzi on 2017/10/24.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMPROCESSTHREAD_H
#define IOTEVENTMONITORPLATFORM_CUSTOMPROCESSTHREAD_H

#include <QThread>
#include <string>
using std::string;

class CustomProcessThread : public QThread {
  Q_OBJECT

  public:
    void stop();
    void setHostIp(const string &hostIp);

  signals:
    void sendStatusMessage(const QString&);

  protected:
    void run() override ;

  private:
    bool threadStop = false;
    /**
     * 远程宿主机的ip
     */
    string hostIp;
    /**
     * 连接远程宿主机
     * @return
     */
    bool connectToHost();
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMPROCESSTHREAD_H
