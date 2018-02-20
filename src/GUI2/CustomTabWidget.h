//
// Created by chenkuan on 2018/2/20.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMTABWIDGET_H


#include <QtWidgets/QTabWidget>
#include <CustomEventManagerWidget.h>
#include <CustomRunWidget.h>
#include <QtWidgets/QStatusBar>
#include <Logger.h>

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

    Logger *logger = Logger::getLogger();

    /**
    * 记录当前width，用于在重绘事件中判断是否要重绘
    */
    int currentWidth = 0;

    /**
     * 重绘事件，判断宽度是否发生变化，变化则调用函数使得标签铺满TabWidget
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

    /**
    * 重绘标签，使得标签铺满TabWidget
    */
    void changeTabStyle();
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMTABWIDGET_H
