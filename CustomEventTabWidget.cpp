//
// Created by yingzi on 2017/10/6.
//

#include <QtGui/QPainter>
#include <qevent.h>
#include <string>
#include <c++/iostream>
#include "CustomEventTabWidget.h"

CustomEventTabWidget::CustomEventTabWidget(QWidget *parent) : QTabWidget(parent) {
    addEventTabButton = new CustomEventWidget(parent);
    this->addTab(addEventTabButton, "+");
    this->setTabPosition(West);

    QObject::connect(this, SIGNAL(tabBarClicked(int)), this, SLOT(addEventTab(int)));
}

void CustomEventTabWidget::paintEvent(QPaintEvent *event) {
    if (saveButton == nullptr) {
        saveButton = new QRect(0, height() - EVENT_TAB_HEIGHT, EVENT_TAB_WIDTH, EVENT_TAB_HEIGHT);
    }
    if (deleteButton == nullptr) {
        deleteButton = new QRect(0, height() - 2 * EVENT_TAB_HEIGHT, EVENT_TAB_WIDTH, EVENT_TAB_HEIGHT);
    }

    QPainter painter(this);
    painter.drawRect(*saveButton);
    painter.drawRect(*deleteButton);
    painter.drawText(*saveButton, Qt::AlignCenter, "保存");
    painter.drawText(*deleteButton, Qt::AlignCenter, "删除");

    QTabWidget::paintEvent(event);

    this->setStyleSheet((
                                "QTabWidget::pane {"
                                        "border: none;"
                                        "}"
                                        "QTabWidget::tab-bar {"
                                        "border: none;"
                                        "}"
                                        "QTabBar::tab {"
                                        "min-width: " + std::to_string(EVENT_TAB_WIDTH) + "px;"
                                        "height: " + std::to_string(EVENT_TAB_HEIGHT) + "px;"
                                        "}"
                        ).c_str());
}

void CustomEventTabWidget::addEventTab(int clickedTab) {
    if (clickedTab + 1 == this->count()) {
        this->insertTab(clickedTab, new CustomEventWidget((QWidget*)this->parent()), ( std::to_string(clickedTab + 1)).c_str());
    }
}

void CustomEventTabWidget::mousePressEvent(QMouseEvent *event) {
    QPoint pos = event->pos();
    if (saveButton->contains(pos)) {
        std::cout << "save" << std::endl;
    }
    else if (deleteButton->contains(pos)) {
        std::cout << "delete" << std::endl;
    }
    QWidget::mousePressEvent(event);
}
