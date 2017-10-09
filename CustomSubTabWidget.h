//
// Created by yingzi on 2017/10/8.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H

#include <string>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include "ConstStyle.h"

/**
 * 事件窗口与状态机窗口TabWidget共同部分的重写
 */
class CustomSubTabWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomSubTabWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);

  protected:
    /**
    * 左侧列表
    */
    QListWidget *listWidget;
    /**
     * 右侧的stackedWidget
     */
    QStackedWidget *stackedWidget;
    /**
    * 整体采用左右布局
    */
    QHBoxLayout *hBoxLayout;
    /**
     * 左侧采用上下布局
     */
    QVBoxLayout *leftVBoxLayout;
    /**
     * 公有的保存和删除按钮
     */
    QPushButton *saveButton;
    QPushButton *deleteButton;
    /**
     * 为listWidget和stackedWidget在最后一页添加选项卡
     */
    virtual void addCustomTab() = 0;

  protected slots:
    /**
     * 接收左侧list更换行信号
     * 如果点击了+号，插入一个选项卡，并将当前行置于非+号上
     * 如果点击了非+号，切换选项卡
     * @param row 点击的list行号
     */
    void changeListRow(int row);
    /**
     * 接收子widget发来的状态栏消息并转发到外层
     * @param message
     */
    void recvStatusMessage(const QString& message);
    /**
     * 删除当前选项卡
     */
    void deleteCurrentTab();
    /**
     * 保存当前选项卡中的内容，交由子类具体实现
     */
    virtual void saveCurrentTab() = 0;

  private:
    /**
    * 重绘标签，更改标签宽度高度等样式
    */
    void changeTabStyle();
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H
