//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H

#include <QTabWidget>
#include "CustomEventTabWidget.h"
#include "CustomStateTabWidget.h"
#include "CustomRunWidget.h"
#include "ConstStyle.h"

/**
 * 中央tabWidget的重写
 */
class CustomMainTabWidget : public QTabWidget{
  Q_OBJECT

  public:
    explicit CustomMainTabWidget(QWidget *parent = nullptr);
    /**
     * 重绘标签，使得标签铺满TabWidget
     * @param event
     */
    void paintEvent(QPaintEvent* event) override ;

  signals:
    void sendStatusMessage(const QString&);

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

  private slots:
    void recvStatusMessage(const QString& message);
};


#endif //IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H
