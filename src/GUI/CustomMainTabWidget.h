//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H

#include <QTabWidget>
#include <QXmlStreamReader>
#include <QCloseEvent>
#include "CustomEventTabWidget.h"
#include "CustomStateTabWidget.h"
#include "CustomRunWidget.h"
#include "ConstStyle.h"
#include <tinyxml2.h>
using namespace tinyxml2;

/**
 * 中央tabWidget的重写
 */
class CustomMainTabWidget : public QTabWidget{
  Q_OBJECT

  public:
    /**
     * 载入配置文件
     * @param parent
     */
    explicit CustomMainTabWidget(QWidget *parent = nullptr);
    /**
     * 被调用时收集各subWidget配置信息保存到文件
     */
    void saveConf();

  signals:
    void sendStatusMessage(const QString&);

  protected:
    /**
    * 重绘事件，判断宽度是否发生变化，变化则调用函数使得标签铺满TabWidget
    * @param event
    */
    void paintEvent(QPaintEvent* event) override ;

  private:
    /**
     * 中央tabWidget只包含三个widget
     */
    CustomSubTabWidget *eventTabWidget;
    CustomSubTabWidget *stateTabWidget;
    CustomRunWidget *runWidget;
    /**
     * 记录当前width，用于在重绘事件中判断是否要重绘
     */
    int currentWidth = 0;
    /**
    * 重绘标签，使得标签铺满TabWidget
    */
    void changeTabStyle();

    /**
     * GUI界面的记忆配置读取存放于此
     */
    XMLDocument GUIConf;
    XMLElement *eventsConf;
    XMLElement *stateMachinesConf;
    XMLElement *runConf;
    /**
     * 从配置文件中读取配置保存至GUIConf
     * 无配置文件则创建默认配置文件
     */
    void readConf();
    /**
     * 解析配置文件
     * @return 解析结果
     */
    bool parseConf(const char *confStr);
    /**
     * 写入配置到文件中
     * @param confStr 配置字符串
     */
    void writeConf(const char *confStr);

  private slots:
    void recvStatusMessage(const QString& message);
};


#endif //IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H
