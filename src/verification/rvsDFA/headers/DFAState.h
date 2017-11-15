//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_DFASTATE_H
#define RUNTIME_VERIFICATION_SYSTEM_DFASTATE_H

#include <State.h>
#include <vector>
#include <z3++.h>

using std::vector;
using z3::expr;
using z3::solver;

class Tran;
class Event;

class DFAState : public State {

  public:
    DFAState();

    ~DFAState() override = default;

    void setStateNum(int stateNum) override;

    void addExpr(expr &exp) override;

    void addTran(Tran *tran) override;

    int getStateNum() const override;

    int getNextState(Event *event, solver &slv) override;

    const vector<expr> &getExps() const override;

    void addValuesExpr(expr &exp) override;

    const vector<expr> &getValuesExps() const override;

    void clearValuesExpr() override;

  private:
    int stateNum;

    vector<expr> exps; // 表达式集合
    vector<Tran *> trans;   // 转移数组

    vector<expr> valuesExps;    // 表达状态实际值的表达式集合
};


#endif //RUNTIME_VERIFICATION_SYSTEM_DFASTATE_H
