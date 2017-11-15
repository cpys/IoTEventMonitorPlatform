//
// Created by chenkuan on 17-11-15.
//

#ifndef IOTEVENTMONITORPLATFORM_STATEPARSER_H
#define IOTEVENTMONITORPLATFORM_STATEPARSER_H

#include <string>
#include <Module.h>
using std::string;

class StateParser {
  public:
    StateParser() = default;
    ~StateParser();

    /**
     * 设置模型的描述xml，
     * @param stateXML
     */
    void setStateXML(const string &stateXML);
    /**
     * 构建模型，解析xml并初始化该模型
     * @return
     */
    bool parseStateXML();
    /**
     * 验证一个字符串格式的事件，返回验证结果
     * 根据事件既定的事件格式解析
     * 调用module的API进行验证
     * @param event
     * @return
     */
    bool validateEvent(const string &event);

  private:
    string stateXML;
    Module *module;

    /**
     * 解析变量声明的原始格式字符串，提取出变量和其类型
     * 添加到模型中
     * @param varDecl
     * @return
     */
    bool parseVarDecl(const char *varDecl);
    /**
     * 解析状态结点上的原始格式字符串，提取出节点编号和表达式
     * 添加到模型中
     * @param state
     * @return
     */
    bool parseState(const char *state);
    /**
     * 解析转移上的原始格式字符串，提取出转移名称和表达式
     * 添加到模型中
     * @param tran
     * @return
     */
    bool parseTran(const char *tran, const char *source, const char *target);
    /**
     * 解析验证原始字符串，提取表达式
     * 添加到模型中
     * @param spec
     * @return
     */
    bool parseSpec(const char *spec);
};


#endif //IOTEVENTMONITORPLATFORM_STATEPARSER_H
