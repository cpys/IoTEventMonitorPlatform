//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_EVENT_H
#define RUNTIME_VERIFICATION_SYSTEM_EVENT_H

#include <string>
#include <vector>
#include <map>
#include <z3++.h>

using std::string;
using std::vector;
using std::map;
using z3::expr;

class Event {

  public:
    Event() = default;

    virtual ~Event() = default;

    /*
     * 设置事件名称
     * @param 事件名称
     */
    virtual void setEventName(const string &) = 0;

    /*
     * 设置变量内容
     * @param 变量内容
     */
    virtual void setEventVars(const map<string, string> &) = 0;

    /*
     * 添加expr
     * @param expr
     */
    virtual void addExpr(expr &) = 0;

    /*
     * 获取事件名称
     * @return 事件名称
     */
    virtual const string &getEventName() const = 0;

    /*
     * 获取expr集合
     * @return 表达式集合
     */
    virtual const vector<expr> &getExps() const = 0;

    /*
     * 输出事件内容
     * @return 事件内容字符串
     */
    virtual const string toString() const = 0;

};


#endif //RUNTIME_VERIFICATION_SYSTEM_EVENT_H
