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
     * @param conf
     */
    void setConf(XMLElement *conf);
    /**
     * 将当前页面配置更新到XMLElement*中
     */
    void saveConfToXML();

  signals:
    void sendStatusMessage(const QString&);

  private:
    QGridLayout *gridLayout = nullptr;

    // 事件与状态机选择下拉框
    QComboBox *eventComboBox = nullptr;
    QComboBox *stateComboBox = nullptr;
    QLabel *eventLabel = nullptr;
    QLabel *stateLabel = nullptr;

    // 事件与状态机预览
    QTextBrowser *eventTextBrowser = nullptr;
    QGraphicsView *stateGraphicsView = nullptr;

    // 网络端口监控配置
    CustomIpEdit *vmIpEdit = nullptr;
    CustomIpEdit *hostIpEdit = nullptr;
    CustomIpEdit *externalIpEdit = nullptr;
    QLabel *vmIpLabel = nullptr;
    QLabel *hostIpLabel = nullptr;
    QLabel *externalIpLabel = nullptr;

    // 串口监控配置
    QLineEdit *pseudoTerminalEdit = nullptr;
    QLineEdit *serialPortEdit = nullptr;
    QLabel *pseudoTerminalLabel = nullptr;
    QLabel *serialPortLabel = nullptr;

    // 虚拟机内存进程监控配置
    QLineEdit *vmNameEdit = nullptr;
    QLineEdit *vmPidEdit = nullptr;
    QLabel *vmNameLabel = nullptr;
    QLabel *vmPidLabel = nullptr;

    // 启动与展示
    QPushButton *runButton = nullptr;
    QTextBrowser *eventTraceTextBrowser = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_RUNWIDGET_H
