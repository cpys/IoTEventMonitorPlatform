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

/**
 * 事件窗口TabWidget的重写
 */
class CustomEventTabWidget : public QTabWidget {
  Q_OBJECT

  public:
    explicit CustomEventTabWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event) override ;
    void mousePressEvent(QMouseEvent *event) override ;

  private slots:
    /**
     * 接收单击添加事件按钮，判断是否点击了+号
     * 如果点击了+号，在最后插入一个选项卡
     * @param clickedTab 点击的选项卡编号
     */
    void addEventTab(int clickedTab);

  private:
    CustomEventWidget *addEventTabButton;
    QRect *saveButton = nullptr;
    QRect *deleteButton = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_EVENTTABWIDGET_H
