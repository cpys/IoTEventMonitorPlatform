//
// Created by yingzi on 2017/11/30.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_STATE_H
#define RUNTIME_VERIFICATION_SYSTEM_STATE_H

#include <vector>
#include <string>
#include <z3++.h>

class Tran;

using std::vector;
using std::string;
using Z3Expr = z3::expr;

class State {
  public:
    State() = default;
    ~State() = default;

    /**
     * 设置节点的编号
     * @param stateNum 节点编号
     */
    void setStateNum(int stateNum);
    /**
     * 设置该节点为起始节点
     */
    void setStartFlag(bool startFlag);
    /**
     * 设置该节点为终止节点
     */
    void setEndFlag(bool endFlag);
    /**
     * 向节点中添加Z3表达式
     * @param z3Expr
     */
    void addZ3Expr(const Z3Expr &z3Expr);
    /**
     * 向节点添加可到达下一状态的转移关系
     * @param tran 相邻出转移
     */
    void addTran(const Tran *tran);

    /**
     * 判断该节点是否为虚拟出的空节点
     * @return 是否为虚拟的空节点
     */
    bool isEmpty() const;
    /**
     * 获取节点编号
     * @return 节点编号
     */
    int getStateNum() const;
    /**
     * 判断是否为起始节点
     * @return 是否为起始节点
     */
    bool isStartState() const;
    /**
     * 判断是否为终止节点
     * @return 终止节点
     */
    bool isEndState() const;
    /**
     * 获取Z3表达式列表
     * @return Z3表达式列表
     */
    const vector<Z3Expr> &getZ3ExprList() const;
    /**
     * 获取可到达下一状态的转移关系列表
     * @return 相邻转移列表
     */
    const vector<const Tran *> &getTranList() const;

    /**
     * 清空节点
     */
    void clear();
  private:
    /**
     * 节点是否为空的标记，只有设置节点编号后才置false
     */
    bool emptyFlag = true;
    /**
     * 节点编号
     */
    int stateNum = 0;
    /**
     * 是否为起始节点
     */
    bool startFlag = false;
    /**
     * 是否为终止节点
     */
    bool endFlag = false;
    /**
     * 节点中的Z3表达式列表
     */
    vector<Z3Expr> z3ExprList;
    /**
     * 节点的所有相邻出转移
     */
    vector<const Tran *> outTranList;

};


#endif //RUNTIME_VERIFICATION_SYSTEM_STATE_H
