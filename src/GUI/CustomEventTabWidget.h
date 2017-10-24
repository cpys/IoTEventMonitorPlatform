//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_EVENTTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_EVENTTABWIDGET_H

#include <string>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include "CustomEventWidget.h"
#include "ConstStyle.h"
#include "CustomSubTabWidget.h"

/**
 * 事件窗口TabWidget的重写
 */
class CustomEventTabWidget : public CustomSubTabWidget {
  Q_OBJECT

  public:
    /**
     * 构造函数中需要添加默认的eventWidget
     * @param parent
     */
    explicit CustomEventTabWidget(QWidget *parent = nullptr);
    void setConf(XMLElement *eventsConf) override ;
    void saveConfToXML() override ;

  protected slots:
    void addCustomTab() override ;

  private:
    /**
    * 当前页的配置
    */
    XMLElement *eventsConf;
};


#endif //IOTEVENTMONITORPLATFORM_EVENTTABWIDGET_H
