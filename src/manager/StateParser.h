//
// Created by chenkuan on 17-11-15.
//

#ifndef IOTEVENTMONITORPLATFORM_STATEPARSER_H
#define IOTEVENTMONITORPLATFORM_STATEPARSER_H

#include <string>
#include <Model.h>
#include <Logger.h>

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
     * 调用model的API进行验证
     * @param event
     * @return
     */
    enum VerificationResult validateEvent(const char *event);
    /**
     * 获取事件是否是关键事件
     * @return
     */
    bool getIsEventImportant();
    /**
     * 不验证事件，只判断是否为关键事件，仅供测试使用
     * @return
     */
    bool justGetIsEventImportant(const string &event);

  private:
    string stateXML;
    Model *model = nullptr;
    /**
     * mxCell的id与节点实际id的映射
     */
    map<int, int> idMap;
    /**
     * 事件是否是关键事件
     */
    bool isEventImportant;

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
     * @param stateGraphId 表示图的xml中节点mxcell的id
     * @return
     */
    bool parseState(const char *state, const char *stateGraphId, bool isEndState);
    /**
     * 解析转移上的原始格式字符串，提取出转移名称和表达式
     * 添加到模型中
     * @param tran
     * @param source
     * @param target
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

    Logger *logger = Logger::getLogger();

    const vector<const char *> IMPORTANT_EVENT_NAME_LIST = {
            "increase",
            "decrease"
    };
};


#endif //IOTEVENTMONITORPLATFORM_STATEPARSER_H
