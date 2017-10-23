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
#include <string>

using std::vector;
using std::string;

class CustomIpEdit : public QWidget {
  Q_OBJECT

  public:
    explicit CustomIpEdit(QWidget *parent = nullptr);
    /**
     * 输入ip点分10进制字符串，解析并填充到输入框中
     * @param ip
     */
    void setIp(const string& ip);
    /**
     * 返回点分10进制的ip字符串
     * @return
     */
    string getIp();

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
