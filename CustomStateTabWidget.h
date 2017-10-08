//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_STATETABWIDGET_H
#define IOTEVENTMONITORPLATFORM_STATETABWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include "CustomStateWidget.h"
#include "CustomSubTabWidget.h"

/**
 * 状态机窗口TabWidget的重写
 */
class CustomStateTabWidget : public CustomSubTabWidget {
  Q_OBJECT

  public:
    /**
     * 构造函数中需要添加默认的stateWidget
     * @param parent
     */
    explicit CustomStateTabWidget(QWidget *parent = nullptr);
    /**
     * 事件窗口需要重绘出“删除”、“查看XML”、“保存”按钮
     * @param event
     */
    void paintEvent(QPaintEvent* event) override ;
    /**
     * 鼠标点击事件响应需要对重绘的三个按钮进行响应
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
     * “删除”、“查看XML”、“保存”的位置
     */
    QRect *deleteButton = nullptr;
    QRect *viewXMLButton = nullptr;
    QRect *saveButton = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_STATETABWIDGET_H
