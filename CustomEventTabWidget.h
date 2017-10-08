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
#include "CustomSubTabWidget.h"

/**
 * 事件窗口TabWidget的重写
 */
class CustomEventTabWidget : public CustomSubTabWidget {
  Q_OBJECT

  public:
    /**
     * 构造函数中需要添加默认的eventWidget
     * @param parent
     */
    explicit CustomEventTabWidget(QWidget *parent = nullptr);
    /**
     * 事件窗口需要重绘出删除和保存按钮
     * @param event
     */
    void paintEvent(QPaintEvent* event) override ;
    /**
     * 鼠标点击事件响应需要对重绘的删除和保存按钮进行响应
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override ;

  private slots:
    /**
     * 接收单击添加事件按钮，判断是否点击了+号
     * 如果点击了+号，在最后插入一个选项卡
     * @param clickedListWidgetItem 点击的选项卡编号
     */
    void clickListWidget(QListWidgetItem *clickedListWidgetItem) override ;

  private:
    /**
     * 删除按钮和保存按钮的位置
     */
    QRect *deleteButton = nullptr;
    QRect *saveButton = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_EVENTTABWIDGET_H
