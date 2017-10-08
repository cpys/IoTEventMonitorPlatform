//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_RUNWIDGET_H
#define IOTEVENTMONITORPLATFORM_RUNWIDGET_H

#include <QtWidgets/QWidget>

/**
 * 运行展示窗口Widget的重写
 */
class CustomRunWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomRunWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);
};


#endif //IOTEVENTMONITORPLATFORM_RUNWIDGET_H
