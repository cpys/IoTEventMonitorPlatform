//
// Created by chenkuan on 2018/2/24.
//

#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit(parent) {

}

void CustomLineEdit::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QLineEdit::mousePressEvent(event);
}
