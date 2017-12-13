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
#include "GUIStyle.h"
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
    virtual void setConf(XMLElement *) = 0;
    /**
     * 将当前页的配置更新到XMLElement*中，交由具体的页面执行
     */
    virtual void saveConfToXML() = 0;

    const QListWidget *getListWidget();
    const QStackedWidget *getStackedWidget();

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
     * 公有的增加/删除按钮
     */
    QPushButton *deleteButton;
    QPushButton *addButton;

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

  private:
    /**
    * 重绘标签，更改标签宽度高度等样式
    */
    void changeTabStyle();
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMSUBTABWIDGET_H
