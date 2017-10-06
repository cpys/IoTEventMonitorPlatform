//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <string>
#include "CustomEventTabWidget.h"

CustomEventTabWidget::CustomEventTabWidget(QWidget *parent) : QTabWidget(parent) {
    addEventTabButton = new CustomEventWidget(parent);
    this->addTab(addEventTabButton, "+");
    this->setTabPosition(West);

    QObject::connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(addEventTab(int)));
}

void CustomEventTabWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
//    QRect rectAdd(width()-m_tablepic.width(),tabBar()->rect().bottom()-m_tablepic.height()+7,m_tablepic.width(),m_tablepic.height()-5);
//    painter.drawImage(rectAdd,m_tablepic);
    painter.drawRect(0, height() - 30, 30, 30);
    painter.drawText(0, height() - 30, "haha");
    QTabWidget::paintEvent(event);
}

void CustomEventTabWidget::addEventTab(int clickedTab) {
    if (clickedTab + 1 == this->count()) {
        this->insertTab(clickedTab, new CustomEventWidget((QWidget*)this->parent()), ( std::to_string(clickedTab + 1)).c_str());
    }
}
