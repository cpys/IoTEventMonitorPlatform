//
// Created by yingzi on 2017/10/24.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMPROCESSTHREAD_H
#define IOTEVENTMONITORPLATFORM_CUSTOMPROCESSTHREAD_H

#include <QThread>
#include <string>
#include "NetfilterClient.h"
using std::string;

class CustomProcessThread : public QThread {
  Q_OBJECT

  public:
    void stop();

  signals:
    void sendStatusMessage(const QString&);

  protected:
    void run() override ;

  private:
    bool threadStop = false;
    NetfilterClient *netfilterClient = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMPROCESSTHREAD_H
