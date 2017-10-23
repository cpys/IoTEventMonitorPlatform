//
// Created by yingzi on 2017/10/23.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMMAINWINDOW_H
#define IOTEVENTMONITORPLATFORM_CUSTOMMAINWINDOW_H


#include <QtWidgets/QMainWindow>
#include <QStatusBar>
#include <QCloseEvent>
#include "CustomMainTabWidget.h"

class CustomMainWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit CustomMainWindow(QWidget *parent = nullptr);

  protected:
    /**
     * 重写关闭窗口事件，在关闭前进行配置保存
     * @param event
     */
    void closeEvent(QCloseEvent *event) override ;

  private:
    CustomMainTabWidget *centralWidget;
    QStatusBar *statusBar;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMMAINWINDOW_H
