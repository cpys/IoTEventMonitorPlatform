//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_STATETABWIDGET_H
#define IOTEVENTMONITORPLATFORM_STATETABWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include "CustomStateWidget.h"

/**
 * 状态机窗口TabWidget的重写
 */
class CustomStateTabWidget : public QTabWidget {
  Q_OBJECT

  public:
    explicit CustomStateTabWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event) override ;

  private slots:
    /**
     * 接收单击添加事件按钮，判断是否点击了+号
     * 如果点击了+号，在最后插入一个选项卡
     * @param clickedTab 点击的选项卡编号
     */
    void addStateTab(int clickedTab);

  private:
    CustomStateWidget *addStateTabButton;

};


#endif //IOTEVENTMONITORPLATFORM_STATETABWIDGET_H
