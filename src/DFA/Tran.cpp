//
// Created by yingzi on 2017/11/30.
//

#include "Tran.h"

void Tran::setTranName(const string &tranName) {
    this->tranName = tranName;
}

void Tran::setSourceState(const State *sourceState) {
    this->sourceState = sourceState;
}

void Tran::setDestState(const State *destState) {
    this->destState = destState;
}

const string &Tran::getTranName() const {
    return tranName;
}

const State *Tran::getSourceState() const {
    return sourceState;
}

const State *Tran::getDestState() const {
    return destState;
}
