//
// Created by chenkuan on 17-5-29.
//

#include <DFAModule.h>
#include <regex>
#include <DFAState.h>
#include <DFATran.h>
#include <DFASpec.h>
#include <DFAEvent.h>

using std::cout;
using std::endl;
using std::cerr;
using std::stack;
using namespace z3;

DFAModule::~DFAModule() {
    for (auto &kv : this->states) {
        if (kv.second != nullptr) {
            delete (kv.second);
            kv.second = nullptr;
        }
    }
    for (auto &tran : this->trans) {
        if (tran != nullptr) {
            delete(tran);
            tran = nullptr;
        }
    }
    for (auto &spec : this->specs) {
        if (spec != nullptr) {
            delete (spec);
            spec = nullptr;
        }
    }
    slv.reset();
}

void DFAModule::addVarDecl(const string &varType, const string &varName) {
    this->varsDecl[varName] = varType;
}

void DFAModule::addState(int stateNum, const vector<string> &stateConstraints) {
    State *state = this->states[stateNum];
    if (state != nullptr) {
        delete (state);
    }

    // 构造一个状态类对象
    state = new DFAState();
    state->setStateNum(stateNum);
    for (auto &stateConstraint : stateConstraints) {
        // 将状态的约束字符串解析成expr，添加到状态中expr集合中
        expr exp = this->extractExpr(stateConstraint);
        state->addExpr(exp);
    }

    // 将状态添加到模型中
    this->states[stateNum] = state;
}

void DFAModule::addTran(const string &tranName, int sourceStateNum, int destStateNum,
                        const vector<string> &tranConstraints) {
    // 取出源状态编号对应的状态
    State *sourceState = this->states[sourceStateNum];
    if (sourceState == nullptr) {
        cerr << "不存在源状态" << sourceStateNum << endl;
        return;
    }

    State *destState = this->states[destStateNum];
    if (destState == nullptr) {
        cerr << "不存在目标状态" << destStateNum << endl;
    }

    // 构造一个转移类对象
    Tran *tran = new DFATran();
    tran->setName(tranName);
    tran->setSourceState(sourceState);
    tran->setDestState(destState);
    // 将转移约束字符串解析成expr,添加到转移中expr集合中
    for (auto &tranConstraint : tranConstraints) {
        expr exp = this->extractExpr(tranConstraint);
        tran->addExpr(exp);
    }

    // 将构造完的转移类添加到源state和module中
    sourceState->addTran(tran);
    this->trans.push_back(tran);
}

void DFAModule::addSpec(const vector<string> &tempConstraints) {
    // 构造一个Spec类对象
    Spec *spec = new DFASpec();
    // 将判定逻辑的约束字符串解析成expr,添加到Spec对象的expr集合中
    for (auto &constraint : tempConstraints) {
        spec->addConstraint(constraint);
        expr exp = this->extractExpr(constraint);
        spec->addExpr(exp);
    }

    // 将Spec类对象添加到module中
    this->specs.push_back(spec);
}

bool DFAModule::addEvent(const string &eventName, const map<string, string> &vars) {
    // 构造一个Event类对象
    Event *event = new DFAEvent();
    event->setEventName(eventName);
    event->setEventVars(vars);

    for (auto &kv : vars) {
        if (this->varsDecl.find(kv.first) == this->varsDecl.end()) {
            cerr << "未声明变量" << kv.first << endl;
            continue;
        }
        string varType = varsDecl[kv.first];
        // 根据变量类型将变量值转换成expr添加到Event类对象中
        if (varType == "int") {
            expr exp_var = this->ctx.int_const(kv.first.c_str());
            expr exp = (exp_var == this->ctx.int_val(kv.second.c_str()));
            event->addExpr(exp);
        } else if (varType == "double") {
            expr exp_var = this->ctx.real_const(kv.first.c_str());
            expr exp = (exp_var == this->ctx.real_val(kv.second.c_str()));
            event->addExpr(exp);
        } else {
            cerr << "变量类型" << varType << "不支持" << endl;
            continue;
        }

    }

    // 每次添加事件时进行轨迹描绘，即进行事件的转移
    if (!this->trace(event)) {
        cerr << "事件" << event->toString() << "转移失败!" << endl;
        return false;
    }

    // 状态转移成功后为状态添加新值表达式
    State *state = this->states[this->currentStateNum];
    if (state == nullptr) {
        cerr << "不存在新状态" << this->currentStateNum << endl;
        return false;
    }
    // 先清空状态中已有的实际值表达式
    state->clearValuesExpr();
    for (auto &kv : vars) {
        if (this->varsDecl.find(kv.first) == this->varsDecl.end()) {
            cerr << "未声明变量" << kv.first << endl;
            continue;
        }
        string varType = varsDecl[kv.first];
        if (varType == "int") {
            expr exp_var = this->ctx.int_const((kv.first + std::to_string(this->currentStateNum)).c_str());
            expr exp = (exp_var == this->ctx.int_val(kv.second.c_str()));
            state->addValuesExpr(exp);
        } else if (varType == "double") {
            expr exp_var = this->ctx.real_const((kv.first + std::to_string(this->currentStateNum)).c_str());
            expr exp = (exp_var == this->ctx.real_val(kv.second.c_str()));
            state->addValuesExpr(exp);
        } else {
            cerr << "变量类型" << varType << "不支持" << endl;
            continue;
        }
    }
    this->stateTracks.push_back(state);
    // 对状态轨迹的跟踪限制长度
    if (this->stateTracks.size() > this->maxStateTracksLength) {
        this->stateTracks.pop_front();
    }

    // 轨迹跟踪完成后对每一条spec进行check
    bool flag = this->check();

//    // 对历史事件进行记录
//    this->events.push_back(event);

    return flag;
}

expr DFAModule::extractExpr(const string &constraint) {
    // 借用运算符栈和运算数栈实现表达式的解析
    stack<string> operatorStack;
    // expr栈用来记录中间表达式结果
    stack<expr> exprStack;

    // 先压入最低优先级运算符
    operatorStack.push("$");

    // 遍历字符串表达式
    string identifier;    // 记录标识符
    string flag;    // 记录当前类型
    for (auto &c : constraint) {
        if (flag == "") {
            // 当前无类型，即可从任意符号开始新的标识符
            if (isalpha(c) != 0 || c == '_') {
                // 当前开始一个新的变量
                flag = "var";
                identifier.push_back(c);
            } else if (isdigit(c) != 0 || c == '.') {
                // 当前开始一个新的运算数
                flag = "operand";
                identifier.push_back(c);
            } else if (isOperator(c)) {
                // 当前为运算符
                flag = "operator";
                identifier.push_back(c);
            } else if (isspace(c) == 0) {
                // 其他情况必须为空格
                cerr << "非法字符" << c << endl;
            }
        } else if (flag == "var") {
            // 当前已处于变量状态
            if (isalnum(c) != 0 || c == '_') {
                // 继续当前变量
                identifier.push_back(c);
            } else {
                // 当前为其他字符则生成完整变量

                // 根据变量名生成变量名expr
                expr exp = generateVarExp(identifier);
                exprStack.push(exp);
                identifier.clear();

                if (isspace(c) != 0) {
                    flag = "";
                } else if (isOperator(c)) {
                    flag = "operator";
                    identifier.push_back(c);
                } else {
                    cerr << "不合法的标识符" << identifier + c << endl;
                }
            }
        } else if (flag == "operand") {
            // 当前已处于运算数状态
            if (isdigit(c) != 0 || c == '.') {
                // 继续当前运算数
                identifier.push_back(c);
            } else {
                // 当前为其他字符则生成完整运算数
                // 分析数的类型得到不同的expr
                expr exp = generateNumExp(identifier);
                exprStack.push(exp);
                identifier.clear();

                if (isspace(c) != 0) {
                    flag = "";
                } else if (isOperator(c)) {
                    flag = "operator";
                    identifier.push_back(c);
                } else {
                    cerr << "非法的标识符" << identifier + c << endl;
                }
            }
        } else if (flag == "operator") {
            // 当前已经处于运算符状态
            if (isOperator(c)) {
                // 继续当前运算符
                identifier.push_back(c);
            } else {
                // 当前为其他字符则生成完整运算符
                // 将当前运算符与栈顶相比较
                while (!compareOperator(identifier, operatorStack.top())) {
                    // 只要当前运算符比栈顶运算符优先级低就一直退两个表达式和一个运算符进行运算后压栈
                    const string operatorTop = operatorStack.top() + "";
                    operatorStack.pop();

                    expr expr2 = exprStack.top();
                    exprStack.pop();
                    expr expr1 = exprStack.top();
                    exprStack.pop();

                    exprStack.push(calcExpr(expr1, operatorTop, expr2));
                }
                // 退出循环时表示当前运算符比栈顶运算符优先级高
                operatorStack.push(identifier);
                identifier.clear();

                if (isalpha(c) != 0 || c == '_') {
                    flag = "var";
                    identifier.push_back(c);
                } else if (isdigit(c) != 0 || c == '.') {
                    flag = "operand";
                    identifier.push_back(c);
                } else if (isspace(c) != 0) {
                    flag = "";
                } else {
                    cerr << "非法字符" << c << endl;
                }
            }
        }
    }

    // 循环结束后需要手动添加结束符, 即循环退栈
    if (flag == "var") {
        // 根据变量名生成变量名expr
        expr exp = generateVarExp(identifier);
        exprStack.push(exp);
        identifier.clear();
    } else if (flag == "operand") {
        // 分析数的类型得到不同的expr
        expr exp = generateNumExp(identifier);
        exprStack.push(exp);
        identifier.clear();
    } else if (flag == "operator") {
        cerr << "表达式" << constraint << "以运算符结尾，非法！" << endl;
    }

    while (operatorStack.size() > 1) {
        string operatorTop = operatorStack.top() + "";
        operatorStack.pop();

        if (exprStack.size() < 2) {
            cerr << "运算数或变量不足，表达式" << constraint << "不合法" << endl;
            break;
        }
        expr expr2 = exprStack.top();
        exprStack.pop();
        expr expr1 = exprStack.top();
        exprStack.pop();

        exprStack.push(calcExpr(expr1, operatorTop, expr2));
    }

    return exprStack.top();
}

bool DFAModule::trace(Event *event) {
//    this->stateNums.clear();
    // 如果当前有状态，则优先从此状态出发判断
    if (this->currentStateNum >= 0) {
        State *currentState = this->states[this->currentStateNum];
        assert(currentState != nullptr);

        // 判断当前状态是否能转移到下一状态
        int nextState = currentState->getNextState(event, this->slv);
        if (nextState >= 0) {
            cout << "事件" << event->toString() << "将当前状态" << this->currentStateNum << "转移到了状态" << nextState << endl;
//            this->stateNums = {this->currentStateNum, nextState};
            this->currentStateNum = nextState;
            return true;
        }
    }

    // 当前无状态或者当前有状态却无法转移到下一状态，则从所有转移中搜索
    string eventName = event->getEventName();
    for (auto &tran : this->trans) {
        // 首先要事件名匹配
        if (tran->getName() == eventName) {
            // 然后判断该转移是否能经由该转移和该事件转移到下一状态
            bool couldTran = tran->checkEvent(event, this->slv);
            if (couldTran) {
                cout << "事件" << event->toString() << "产生了从状态" << tran->getSourceStateNum() << "到状态"
                     << tran->getDestStateNum() << "的转移" << endl;
                this->currentStateNum = tran->getDestStateNum();
//                this->stateNums = {currentTran->getSourceStateNum(), this->currentStateNum};
                return true;
            }
        }
    }

    return false;
}

bool DFAModule::isOperator(const char &c) {
    string operatorStr = "+-*/<>!=";
    return operatorStr.find(c) != string::npos;
}

expr DFAModule::generateVarExp(const string &varName) {
    if (this->varsDecl.find(varName) == this->varsDecl.end()) {
        cerr << "未声明的变量" << varName << endl;
    }
    string varType = this->varsDecl[varName];
    if (varType == "int") {
        return this->ctx.int_const(varName.c_str());
    } else if (varType == "double") {
        return this->ctx.real_const(varName.c_str());
    } else {
        cerr << "变量" << varName << "类型" << varType << "不支持" << endl;
        return this->ctx.string_val(varName);
    }
}

expr DFAModule::generateNumExp(const string &operand) {
    if (operand.find('.') >= 0) return this->ctx.real_val(operand.c_str());
    else return this->ctx.int_val(operand.c_str());
}

bool DFAModule::compareOperator(const string &operator1, const string &operator2) {
    map<string, int> operatorPriority = {
            {"$",  0},
            {"==", 1},
            {"!=", 1},
            {"<",  2},
            {"<=", 2},
            {">",  2},
            {">=", 2},
            {"+",  3},
            {"-",  3},
            {"*",  4},
            {"/",  4}
    };
    if (operatorPriority.find(operator1) == operatorPriority.end()) {
        cerr << "运算符" << operator1 << "不支持" << endl;
        return false;
    }
    if (operatorPriority.find(operator2) == operatorPriority.end()) {
        cerr << "运算符" << operator2 << "不支持" << endl;
        return false;
    }
    return operatorPriority[operator1] > operatorPriority[operator2];
}

expr DFAModule::calcExpr(expr &expr1, const string &currentOperator, expr &expr2) {
    if (currentOperator == "==") return expr1 == expr2;
    if (currentOperator == "!=") return expr1 != expr2;
    if (currentOperator == "<") return expr1 < expr2;
    if (currentOperator == "<=") return expr1 <= expr2;
    if (currentOperator == ">") return expr1 > expr2;
    if (currentOperator == ">=") return expr1 >= expr2;
    if (currentOperator == "+") return expr1 + expr2;
    if (currentOperator == "-") return expr1 - expr2;
    if (currentOperator == "*") return expr1 * expr2;
    if (currentOperator == "/") return expr1 / expr2;
    else {
        cerr << "不支持的运算符" << currentOperator << endl;
        return expr1;
    }
}

bool DFAModule::check() {
    bool flag = true;
    // 检查每一条spec
    for (auto &spec : this->specs) {
        // 先将状态轨迹中的所有表达式添加到solver中
        this->slv.reset();
        for (auto &state : this->stateTracks) {
            for (auto &exp : state->getValuesExps()) {
                this->slv.add(exp);
            }
        }

        // 先添加spec的表达式
        for (auto &exp : spec->getExps()) {
            this->slv.add(exp);
        }

        z3::check_result result = this->slv.check();
        if (result == z3::sat) {
            cout << "验证逻辑" << spec->toString() << "通过验证" << endl;
        }
        else if (result == z3::unsat) {
            cout << "验证逻辑" << spec->toString() << "验证失败" << endl;
            flag = false;
        }
    }

    return flag;
}

void DFAModule::initModule() {
    map<string, string> newVarsDecl;    // 包含编号的变量声明，变量名：类型
    // 遍历原有的变量声明，同时遍历声明的状态编号，为可能的带编号变量名添加声明
    for (auto &kvVarDecl : this->varsDecl) {
        newVarsDecl[kvVarDecl.first] = kvVarDecl.second;
        for (auto &kvState : this->states) {
            newVarsDecl[kvVarDecl.first + std::to_string(kvState.first)] = kvVarDecl.second;
        }
    }
    this->varsDecl = newVarsDecl;
}