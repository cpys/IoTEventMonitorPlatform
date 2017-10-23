//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_RUNWIDGET_H
#define IOTEVENTMONITORPLATFORM_RUNWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include "CustomIpEdit.h"
#include <tinyxml2.h>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidget>

using namespace tinyxml2;

/**
 * 运行展示窗口Widget的重写
 */
class CustomRunWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomRunWidget(QWidget *parent = nullptr);
    /**
     * 根据conf来配置当前页面
     * @param runConf
     */
    void setConf(XMLElement *runConf);
    /**
     * 将当前页面配置更新到XMLElement*中
     */
    void saveConfToXML();
    /**
     * 保存来自配置页的列表
     * @param listWidget
     * @param stackedWidget
     */
    void setEventList(const QListWidget *listWidget, const QStackedWidget *stackedWidget);
    void setStateList(const QListWidget *listWidget, const QStackedWidget *stackedWidget);

  signals:
    void sendStatusMessage(const QString&);

  private:
    QGridLayout *gridLayout = nullptr;

    // 事件与状态机选择下拉框
    QLabel *eventLabel = nullptr;
    QLabel *stateLabel = nullptr;
    QComboBox *eventComboBox = nullptr;
    QComboBox *stateComboBox = nullptr;

    // 事件与状态机预览
    QTextBrowser *eventTextBrowser = nullptr;
    QGraphicsView *stateGraphicsView = nullptr;

    // 事件跟踪展示
    QTextBrowser *eventTraceTextBrowser = nullptr;

    // 网络端口监控配置
    QLabel *vmIpLabel = nullptr;
    QLabel *externalIpLabel = nullptr;
    CustomIpEdit *vmIpEdit = nullptr;
    CustomIpEdit *externalIpEdit = nullptr;

    // 串口监控配置
    QLabel *pseudoTerminalLabel = nullptr;
    QLabel *serialPortLabel = nullptr;
    QLineEdit *pseudoTerminalEdit = nullptr;
    QLineEdit *serialPortEdit = nullptr;

    // 虚拟机内存进程监控配置
    QLabel *vmNameLabel = nullptr;
    QLabel *vmPidLabel = nullptr;
    QLineEdit *vmNameEdit = nullptr;
    QLineEdit *vmPidEdit = nullptr;

    // 远程的宿主机ip配置
    QLabel *hostIpLabel = nullptr;
    CustomIpEdit *hostIpEdit = nullptr;

    // 启动与展示
    QPushButton *runButton = nullptr;

    /**
     * 记录该页配置的节点
     */
    XMLElement *runConf;

    /**
     * 此两项分别来自前面事件配置页
     */
    const QListWidget *eventListWidget;
    const QStackedWidget *eventStackedWidget;
    /**
     * 此两项分别来自前面状态机配置页
     */
    const QListWidget *stateListWidget;
    const QStackedWidget *stateStackedWidget;

  private slots:
    void updateEventList();
    void updateStateList();

};


#endif //IOTEVENTMONITORPLATFORM_RUNWIDGET_H
