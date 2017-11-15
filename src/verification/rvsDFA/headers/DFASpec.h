//
// Created by chenkuan on 17-5-29.
//

#ifndef RUNTIME_VERIFICATION_SYSTEM_DFASPEC_H
#define RUNTIME_VERIFICATION_SYSTEM_DFASPEC_H

#include <Spec.h>
#include <string>
#include <vector>
#include <z3++.h>

using std::string;
using std::vector;
using z3::expr;

class DFASpec : public Spec {

  public:
    DFASpec() = default;

    ~DFASpec() override = default;

    void addConstraint(const string &constraint) override;

    void addExpr(expr &exp) override;

    const vector<expr> &getExps() const override;

    const vector<string> &getConstraints() const override;

    const string toString() const override;

  private:
    vector<string> constraints;
    vector<expr> exps;
};


#endif //RUNTIME_VERIFICATION_SYSTEM_DFASPEC_H
