//
// Created by yingzi on 2017/11/30.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_TRAN_H
#define RUNTIME_VERIFICATION_SYSTEM_TRAN_H

#include <string>
class State;

using std::string;

class Tran {
  public:
    Tran() = default;
    ~Tran() = default;

    /**
     * 设置转移名称
     * @param tranName 转移名称
     */
    void setTranName(const string &tranName);
    /**
     * 设置源状态
     * @param sourceState 源状态
     */
    void setSourceState(const State *sourceState);
    /**
     * 设置目的状态
     * @param destState 目的状态
     */
    void setDestState(const State *destState);

    /**
     * 获取转移名称
     * @return 转移名称
     */
    const string &getTranName() const;
    /**
     * 获取源状态
     * @return 源状态
     */
    const State *getSourceState() const;
    /**
     * 获取目的状态
     * @return 目的状态
     */
    const State *getDestState() const;

  private:
    /**
     * 转移名称
     */
    string tranName;
    /**
     * 源状态
     */
    const State *sourceState = nullptr;
    /**
     * 目的状态
     */
    const State *destState = nullptr;
};


#endif //RUNTIME_VERIFICATION_SYSTEM_TRAN_H
