//
// Created by chenkuan on 17-5-29.
//

#include <DFAState.h>
#include <Event.h>
#include <Tran.h>

DFAState::DFAState() {
    this->stateNum = -1;
}

void DFAState::setStateNum(int stateNum) {
    this->stateNum = stateNum;
}

void DFAState::addExpr(expr &exp) {
    this->exps.push_back(exp);
}

void DFAState::addTran(Tran *tran) {
    this->trans.push_back(tran);
}

int DFAState::getStateNum() const {
    return this->stateNum;
}

int DFAState::getNextState(Event *event, solver &slv) {
    // 遍历此状态出发的转移关系，依次判断是否符合
    for (auto &tran : this->trans) {
        if (tran->getName() != event->getEventName()) continue;

        slv.reset();
//        // 先添加状态内的expr
//        for (auto &exp : this->exps) {
//            slv.add(exp);
//        }
        // 再添加转移内的expr
        for (auto &exp : tran->getExps()) {
            slv.add(exp);
        }
        // 再添加事件内的expr
        for (auto &exp : event->getExps()) {
            slv.add(exp);
        }
        // 再添加下一状态的expr
        for (auto &exp : tran->getDestState()->getExps()) {
            slv.add(exp);
        }
        if (slv.check() == z3::sat) {
            return tran->getDestStateNum();
        }
    }
    return -1;
}

const vector<expr> &DFAState::getExps() const {
    return this->exps;
}

void DFAState::addValuesExpr(expr &exp) {
    this->valuesExps.push_back(exp);
}

const vector<expr> &DFAState::getValuesExps() const {
    return this->valuesExps;
}

void DFAState::clearValuesExpr() {
    this->valuesExps.clear();
}