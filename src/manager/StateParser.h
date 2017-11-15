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
    bool parseState();
    /**
     * 返回构建的模型
     * @return
     */
    Module *getModule();
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
};


#endif //IOTEVENTMONITORPLATFORM_STATEPARSER_H
