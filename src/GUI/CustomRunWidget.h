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
#include "CustomProcessThread.h"
#include "CustomStateWidget.h"
#include "CustomEventWidget.h"
#include <tinyxml2.h>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QListWidget>
#include <EventManager.h>

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
    /**
     * 更新此页面信息
     */
    void updateWidget();

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
     * 当前选中的事件页
     */
    const CustomEventWidget *currentEventWidget;
    /**
     * 此两项分别来自前面状态机配置页
     */
    const QListWidget *stateListWidget;
    const QStackedWidget *stateStackedWidget;
    /**
     * 后台处理线程，用来与各个客户端连接并验证事件
     */
    EventManager *eventManager;
    /**
     * 更新下拉列表和其对应的显示框
     */
    void updateEventList();
    void updateStateList();
    /**
     * 启动后台处理线程
     */
    void run();
    /**
     * 停止后台处理线程
     */
    void stop();
    /**
     * 改变页面所有可编辑控件的状态
     * @param isEnabled
     */
    void changeWidgetState(bool isEnabled);

  private slots:
    void recvStatusMessage(const QString& message);
    /**
     * 根据下拉框中选中的事件更新显示对应事件
     * @param index
     */
    void showSelectEvent(int index);
    /**
     * 根据下拉框中选中的状态机更新显示对应状态机
     * @param index
     */
    void showSelectState(int index);
    /**
     * 运行按钮事件，根据按钮显示文本来判断当前状态
     * 调用run或stop方法
    */
    void runButtonClicked();

    void showLogMessage(const QString &message);
    /**
     * 后台线程结束事件，进行页面内恢复工作
     */
    void threadFinished();
};


#endif //IOTEVENTMONITORPLATFORM_RUNWIDGET_H
