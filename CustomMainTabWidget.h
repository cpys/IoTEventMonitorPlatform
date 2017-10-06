//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H

#include <QTabWidget>
#include "CustomEventTabWidget.h"
#include "CustomStateTabWidget.h"
#include "CustomRunWidget.h"

/**
 * 中央tabWidget的重写
 */
class CustomMainTabWidget : public QTabWidget{
  public:
    explicit CustomMainTabWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event) override ;

  private:
    CustomEventTabWidget *eventTabWidget;
    CustomStateTabWidget *stateTabWidget;
    CustomRunWidget *runWidget;
};


#endif //IOTEVENTMONITORPLATFORM_MAINTABWIDGET_H
