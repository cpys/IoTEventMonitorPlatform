//
// Created by chenkuan on 2018/2/20.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMRUNWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMRUNWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <CustomIpEdit.h>

class CustomRunWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomRunWidget(QWidget *parent = nullptr);

  private:
    /**
     * 整体采用左右布局，左右可设2:8宽度比
     */
    QHBoxLayout *hBoxLayout = nullptr;

    /**
     * 左侧采用网格布局
     */
    QGridLayout *leftGridLayout = nullptr;
    // 事件选择下拉框
    QLabel *eventLabel = nullptr;
    QComboBox *eventComboBox = nullptr;
    // 事件预览
    QTextBrowser *eventPreviewBrowser = nullptr;
    // 网络监控配置
    QLabel *vmIpLabel = nullptr;
    CustomIpEdit *vmIpEdit = nullptr;
    QLabel *externalIpLabel = nullptr;
    CustomIpEdit *externalIpEdit = nullptr;
    // 串口监控配置
    QLabel *pseudoTerminalLabel = nullptr;
    QLineEdit *pseudoTerminalEdit = nullptr;
    QLabel *serialPortLabel = nullptr;
    QLineEdit *serialPortEdit = nullptr;
    // 状态机文件选择
    QLabel *stateMachineSelectLabel = nullptr;
    QLineEdit *stateMachineEdit = nullptr;

    // 启动与停止
    QPushButton *runButton = nullptr;
    // 清屏按钮
    QPushButton *clearButton = nullptr;

    /**
     * 右侧使用一个QTextBrowser显示信息
     */
    QTextBrowser *logTextBrowser = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMRUNWIDGET_H
