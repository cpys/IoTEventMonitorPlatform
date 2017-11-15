//
// Created by chenkuan on 17-5-29.
//

#include <DFASpec.h>
#include <sstream>

using std::ostringstream;

void DFASpec::addConstraint(const string &constraint) {
    this->constraints.push_back(constraint);
}

void DFASpec::addExpr(expr &exp) {
    this->exps.push_back(exp);
}

const vector<expr> &DFASpec::getExps() const {
    return this->exps;
}

const vector<string> &DFASpec::getConstraints() const {
    return this->constraints;
}

const string DFASpec::toString() const {
    ostringstream oss;
    oss << "{";
    for (auto it = this->constraints.cbegin(); it != this->constraints.cend(); ++it) {
        if (it != this->constraints.cbegin()) oss << ", ";
        oss << "\"" << *it << "\"";
    }
    oss << "}";
    return oss.str();
}