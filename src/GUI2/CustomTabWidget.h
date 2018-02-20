//
// Created by chenkuan on 2018/2/20.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMTABWIDGET_H


#include <QtWidgets/QTabWidget>
#include <CustomEventManagerWidget.h>
#include <CustomRunWidget.h>
#include <QtWidgets/QStatusBar>

/**
 * 主tabWidget，两个tab分别显示事件管理和运行展示页面
 */
class CustomTabWidget : public QTabWidget {
  Q_OBJECT

  public:
    explicit CustomTabWidget(QWidget *parent = nullptr);

  private:
    CustomEventManagerWidget *eventManagerWidget = nullptr;
    CustomRunWidget *runWidget = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMTABWIDGET_H
