//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_MODULE_H
#define RUNTIME_VERIFICATION_SYSTEM_MODULE_H

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class Module {

  public:
    Module() = default;

    virtual ~Module() = default;

    /*
     * 添加变量声明
     * @param 变量类型 变量名
     * 变量类型可取值由z3求解器决定，包括bool, int, real等
     */
    virtual void addVarDecl(const string &, const string &) = 0;

    /*
     * 添加一个状态
     * @param 状态编号 状态约束集
     */
    virtual void addState(int, const vector<string> &) = 0;

    /*
     * 添加一个转移关系
     * @param 转移名称 源状态编号 目的状态编号 转移关系
     */
    virtual void addTran(const string &, int, int, const vector<string> &) = 0;

    /*
    * 初始化模型，目的是允许带编号的变量名称
    */
    virtual void initModule() = 0;

    /*
     * 添加一个判定逻辑
     * @param 判定逻辑
     */
    virtual void addSpec(const vector<string> &) = 0;

    /*
     * 添加一个事件
     * @param 事件名称 事件中变量值<变量名：变量值>
     */
    virtual bool addEvent(const string &, const map<string, string> &) = 0;
};


#endif //RUNTIME_VERIFICATION_SYSTEM_MODULE_H
