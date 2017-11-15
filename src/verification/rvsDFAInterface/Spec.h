//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_SPEC_H
#define RUNTIME_VERIFICATION_SYSTEM_SPEC_H

#include <string>
#include <vector>
#include <z3++.h>

using std::string;
using std::vector;
using z3::expr;

class Spec {

  public:
    Spec() = default;

    virtual ~Spec() = default;

    /*
     * 添加字符串约束
     * @param 字符串约束
     */
    virtual void addConstraint(const string &) = 0;

    /*
     * 添加expr
     * @param expr
     */
    virtual void addExpr(expr &) = 0;

    /*
     * 获取表达式列表
     * @return 表达式列表
     */
    virtual const vector<expr> &getExps() const = 0;

    /*
     * 获取字符串约束
     * @return 字符串约束
     */
    virtual const vector<string> &getConstraints() const = 0;

    /*
     * 获取格式化输出
     * @return 格式化输出
     */
    virtual const string toString() const = 0;
};


#endif //RUNTIME_VERIFICATION_SYSTEM_SPEC_H
