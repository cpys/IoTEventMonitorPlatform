//
// Created by chenkuan on 2018/2/20.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMMAINWINDOW_H
#define IOTEVENTMONITORPLATFORM_CUSTOMMAINWINDOW_H


#include <QtWidgets/QMainWindow>
#include <CustomTabWidget.h>

/**
 * 主窗口类，包括一个tab组件和一个状态栏组件
 */
class CustomMainWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit CustomMainWindow(QWidget *parent = nullptr);

  private:
    CustomTabWidget *tabWidget = nullptr;
    QStatusBar *statusBar = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMMAINWINDOW_H
