//
// Created by yingzi on 2017/11/30.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_MODULE_H
#define RUNTIME_VERIFICATION_SYSTEM_MODULE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <z3++.h>
#include <ctime>

#include "State.h"
#include "Tran.h"

using std::string;
using std::vector;
using std::map;
using std::set;
using z3::context;
using z3::solver;
using Z3Expr = z3::expr;

class Module {
  public:
    Module();
    ~Module();

    /**
     * 添加变量声明
     * @param varType 变量类型
     * @param varName 变量名称
     */
    void addVarDecl(const string &varType, const string &varName);
    /**
     * 添加起始状态节点定义
     * @param stateNum 起始状态节点编号
     * @param stateExprStrList 状态节点上的字符串表达式列表
     */
    void addStartState(int stateNum, const vector<string> &stateExprStrList);
    /**
     * 添加终止状态节点定义
     * @param stateNum 终止状态节点编号
     * @param stateExprStrList 状态节点上的字符串表达式列表
     */
    void addEndState(int stateNum, const vector<string> &stateExprStrList);
    /**
     * 添加状态节点定义
     * @param stateNum 状态节点编号
     * @param stateExprStrList 状态节点上的字符串表达式列表
     */
    void addState(int stateNum, const vector<string> &stateExprStrList);

    /**
     * 设置节点为起始节点
     * @param stateNum 节点编号
     */
    void setStartState(int stateNum);

    /**
     * 设置节点为终止节点
     * @param stateNum 节点编号
     */
    void setEndState(int stateNum);
    /**
     * 添加状态节点转移定义
     * @param tranName 转移名称
     * @param sourceStateNum 源节点
     * @param destStateNum 目的节点
     */
    void addTran(const string &tranName, int sourceStateNum, int destStateNum);
    /**
     * 添加一条轨迹验证约束
     * @param specStr 约束的字符串表达式
     */
    void addSpec(const string &specStr);
    /**
     * 添加事件，返回事件验证结果
     * @param eventName 事件名称
     * @param varValueMap 变量名:变量值 映射表
     * @return 事件验证结果
     */
    bool addEvent(const string &eventName, const map<string, string> &varValueMap);

    /**
     * 初始化模型，进行虚拟空节点检查
     * 对模型是否输入了起始节点和终止节点进行检查
     * 指定当前节点为起始节点
     * @return 初始化结果
     */
    bool initModule();

  private:
    /**
     * 所有变量声明，变量名：类型
     */
    map<string, string> varsDecl;
    /**
     * 状态节点自身编号与实际状态结构映射表
     */
    map<int, State *> states;
    /**
     * 是否输入了起始节点
     */
    bool hasStartState = false;
    /**
     * 是否输入了终止节点
     */
    bool hasEndState = false;
    /**
     * 模型的起始节点
     */
    State *startState = nullptr;
    /**
     * 模型的终止节点
     */
    State *endState = nullptr;
    /**
     * 所有的转移
     */
    vector<Tran *> trans;
    /**
     * 所有的判定逻辑
     */
    vector<Z3Expr> specZ3ExprVector;

    /**
     * 当前状态节点
     */
    State *currentState = nullptr;
    /**
     * 状态节点轨迹
     */
    vector<State *> stateTrace;
    /**
     * 记录当前事件尝试转移过程中失败的节点
     */
    set<const State *> currentFailedStates;

    /**
     * Z3求解器的上下文
     */
    context ctx;
    /**
     * Z3求解器
     */
    solver slv;
//    /**
//     * 所有的变量序号对应的Z3表达式
//     * 变量名称:(变量序号：Z3表达式)
//     */
//    map<string, map<int, Z3Expr> > z3VarsNumExpr;

    /**
     * 将字符串形式的表达式转成Z3表达式
     * 提取出的变量需要分离出变量名称和序号来获取预定的Z3表达式
     * @param exprStr 字符串表达式
     * @param serialNum 变量关联的序号，无则输入""
     * @return 转换成的Z3表达式
     */
    const Z3Expr extractZ3Expr(const string &exprStr, const string &serialNum);
    /**
     * 根据变量名称查找变量类型并生成z3表达式
     * 变量名称末尾需要包含序号
     * @param 变量名
     * @return Z3表达式
     */
    const Z3Expr generateVarExp(const string &varName);
    /**
     * 根据字符串表示的数字生成数字表达式
     * @param 数字字符串
     * @return expr
     */
    const Z3Expr generateNumExp(const string &operand);
    /**
     * 判断字符是否为合法运算符号
     * @param c 运算符号
     * @return 是否合法
     */
    bool isOperator(char c);
    /**
     * 比较两运算符优先级
     * @param 运算符1 运算符2
     * @return 如果运算符1优先级比运算符2优先级高，返回true，否则返回false
     */
    bool compareOperator(const string &operator1, const string &operator2);
    /**
     * 根据两表达式和一个运算符生成新的表达式
     * @param 表达式1 运算符 表达式2
     * @return 运算后的表达式
     */
    const Z3Expr calcExpr(const Z3Expr &expr1, const string &currentOperator, const Z3Expr &expr2);
    /**
     * 验证待定的下一个状态和事件上的变量能否通过验证
     * 验证成功则保留新加入的Z3表达式
     * 验证失败则删除新加入的Z3表达式
     * @param nextState 待定的下一个状态
     * @param varValueMap 事件上的变量名：变量值 映射表
     * @return 验证结果
     */
    bool verify(const State *nextState, const map<string, string> &varValueMap);
};


#endif //RUNTIME_VERIFICATION_SYSTEM_MODULE_H
