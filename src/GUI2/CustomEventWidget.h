//
// Created by chenkuan on 2018/2/20.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTextBrowser>

class CustomEventWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomEventWidget(QWidget *parent = nullptr);

    /**
     * 返回拼接的完整事件内容
     * @return
     */
    QString text();

  signals:

    void eventContentChanged();

  private:
    /**
     * 使用网格布局
     */
    QGridLayout *gridLayout = nullptr;

    QTextEdit *upperOutLayer = nullptr;
    QTextEdit *middleLayer = nullptr;
    QTextEdit *bottomOutLayer = nullptr;
    QTextBrowser *upperIndicator = nullptr;
    QTextBrowser *middleIndicator = nullptr;
    QTextBrowser *bottomIndicator = nullptr;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMEVENTWIDGET_H
