//
// Created by yingzi on 2017/10/8.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H

#include <string>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include "ConstStyle.h"

/**
 * 事件窗口与状态机窗口TabWidget共同部分的重写
 */
class CustomSubTabWidget : public QTabWidget {
  Q_OBJECT

  public:
    explicit CustomSubTabWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);

  protected:
    /**
     * '+'号对应的空widget
     */
    QWidget *emptyTab;
    /**
     * 保存当前高度
     */
    int currentHeight = 0;

  protected slots:
    /**
     * 接收单击添加事件按钮，判断是否点击了+号
     * 如果点击了+号，在最后插入一个选项卡
     * @param clickedTab 点击的选项卡编号
     */
    virtual void addCustomTab(int clickedTab) = 0;

  protected slots:
    void recvStatusMessage(const QString& message);

  private:
    /**
    * 重绘标签，更改标签宽度高度等样式
    */
    void changeTabStyle();
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H
