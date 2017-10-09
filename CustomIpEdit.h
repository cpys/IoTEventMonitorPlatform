//
// Created by yingzi on 2017/10/8.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMIPEDIT_H
#define IOTEVENTMONITORPLATFORM_CUSTOMIPEDIT_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <vector>
#include <QtWidgets/QHBoxLayout>

using std::vector;

class CustomIpEdit : public QWidget {
  Q_OBJECT

  public:
    explicit CustomIpEdit(QWidget *parent = nullptr);

  private:
    QHBoxLayout *hBoxLayout;
    vector<QLineEdit*> ipEdits = vector<QLineEdit*>(4);
    vector<QLabel*> pointLabels = vector<QLabel*>(3);
    /**
     * 修改ip输入框的样式
     */
    void changeStyle();
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMIPEDIT_H
