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
#include <tinyxml2.h>
using namespace tinyxml2;

/**
 * 事件窗口与状态机窗口TabWidget共同部分的重写
 */
class CustomSubTabWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomSubTabWidget(QWidget *parent = nullptr);
    /**
     * 输入配置并使配置在页面上生效，交由具体的页面执行
     * @param conf
     */
    virtual void setConf(XMLElement *conf) = 0;

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
     * 公有的保存和增加/删除按钮
     */
    QPushButton *saveButton;
    QPushButton *deleteButton;
    QPushButton *addButton;
    /**
     * 当前页的配置
     */
    XMLElement *conf;

  protected slots:
    /**
     * 接收子widget发来的状态栏消息并转发到外层
     * @param message
     */
    void recvStatusMessage(const QString& message);
    /**
     * 为listWidget和stackedWidget在最后一页添加选项卡
     */
    virtual void addCustomTab() = 0;
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
