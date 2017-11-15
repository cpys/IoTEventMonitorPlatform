//
// Created by chenkuan on 17-5-29.
//

#include <DFAEvent.h>
#include <sstream>

using std::ostringstream;

void DFAEvent::setEventName(const string &eventName) {
    this->eventName = eventName;
}

void DFAEvent::setEventVars(const map<string, string> &vars) {
    this->vars = vars;
}

void DFAEvent::addExpr(expr &exp) {
    this->exps.push_back(exp);
}

const string &DFAEvent::getEventName() const {
    return eventName;
}

const vector<expr> &DFAEvent::getExps() const {
    return this->exps;
}

const string DFAEvent::toString() const {
    ostringstream oss;
    oss << this->eventName << ": {";
    for (auto it = this->vars.cbegin(); it != this->vars.cend(); ++it) {
        if (it != this->vars.cbegin()) oss << ", ";
        oss << "\"" << it->first << " = " << it->second << "\"";
    }
    oss << "}";
    return oss.str();
}