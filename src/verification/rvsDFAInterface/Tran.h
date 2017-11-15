//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_TRAN_H
#define RUNTIME_VERIFICATION_SYSTEM_TRAN_H

#include <string>
#include <vector>
#include <z3++.h>

using std::string;
using std::vector;
using z3::expr;
using z3::solver;

class State;
class Event;

class Tran {

  public:
    Tran() = default;

    virtual ~Tran() = default;

    /*
     * 设置转移类的名称
     * @param 转移名称
     */
    virtual void setName(const string &) = 0;

    /*
     * 设置转移源状态
     * @param 源状态
     */
    virtual void setSourceState(State *) = 0;

    /*
     * 设置转移目标状态
     * @param 目标状态
     */
    virtual void setDestState(State *) = 0;

    /*
     * 添加expr到expr集合中
     * @param expr
     */
    virtual void addExpr(expr &) = 0;

    /*
     * 获取转移的事件名称
     * @return 转移名称
     */
    virtual const string getName() const = 0;

    /*
     * 获取源状态
     * @return 源状态
     */
    virtual State *getSourceState() const = 0;

    /*
     * 获取目标状态
     * @return 目标状态
     */
    virtual State *getDestState() const = 0;

    /*
     * 获取转移源状态编号
     * @return 源状态编号
     */
    virtual int getSourceStateNum() const = 0;

    /*
     * 获取转移目标状态编号
     * @return 目标状态编号
     */
    virtual int getDestStateNum() const = 0;

    /*
     * 获取表达式集合
     * @return 表达式集合
     */
    virtual const vector<expr> &getExps() const = 0;

    /*
     * 检查对给定事件该转移是否符合
     * @param 事件 求解器
     * @return 该转移是否符合
     */
    virtual bool checkEvent(Event *, solver &) = 0;
};


#endif //RUNTIME_VERIFICATION_SYSTEM_TRAN_H
