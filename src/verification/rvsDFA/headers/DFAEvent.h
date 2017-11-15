//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_DFAEVENT_H
#define RUNTIME_VERIFICATION_SYSTEM_DFAEVENT_H

#include <Event.h>

class DFAEvent : public Event {

  public:
    DFAEvent() = default;

    ~DFAEvent() override = default;

    void setEventName(const string &eventName) override;

    void setEventVars(const map<string, string> &vars) override;

    void addExpr(expr &exp) override;

    const string &getEventName() const override;

    const vector<expr> &getExps() const override;

    const string toString() const override;

  private:
    string eventName;
    map<string, string> vars;
    vector<expr> exps;
};


#endif //RUNTIME_VERIFICATION_SYSTEM_DFAEVENT_H
