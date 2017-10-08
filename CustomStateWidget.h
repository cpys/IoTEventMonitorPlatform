//
// Created by yingzi on 2017/10/6.
//

#ifndef IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H
#define IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>

class CustomStateWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CustomStateWidget(QWidget *parent = nullptr);

  signals:
    void sendStatusMessage(const QString&);

  private:
    QHBoxLayout *vBoxLayout;
    QGraphicsView *canvas;
    QGraphicsView *graphBoard;
};


#endif //IOTEVENTMONITORPLATFORM_CUSTOMSTATEWIDGET_H
