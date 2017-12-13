//
// Created by yingzi on 2017/11/30.
//

#include <stack>
#include "Model.h"
using std::stack;

Model::Model() : slv(ctx) {

}

Model::~Model() {
    for (auto &kv : states) {
        delete (kv.second);
    }
    for (auto &tran : trans) {
        delete (tran);
    }
}

void Model::addVarDecl(const string &varType, const string &varName) {
    this->varsDecl[varName] = varType;
}

void Model::addStartState(int stateNum, const vector<string> &stateExprStrList) {
    this->addState(stateNum, stateExprStrList);
    this->setStartState(stateNum);
}

void Model::addEndState(int stateNum, const vector<string> &stateExprStrList) {
    this->addState(stateNum, stateExprStrList);
    this->setEndState(stateNum);
}

void Model::addState(int stateNum, const vector<string> &stateExprStrList) {
    State *oldState = this->states[stateNum];
    if (oldState == nullptr) {
        oldState = new State();
    } else if (!oldState->isEmpty()) {
        logger->warning("已经添加过编号为%d的节点，将覆盖旧节点", stateNum);
        oldState->clear();
    }

    // 新建状态机节点，将编号和解析成Z3的表达式添加进节点
    State *newState = oldState;
    newState->setStateNum(stateNum);
    for (auto &stateExprStr : stateExprStrList) {
        const Z3Expr z3Expr = this->extractZ3Expr(stateExprStr, std::to_string(stateNum));
        newState->addZ3Expr(z3Expr);
    }

    // 记录或更新此状态到模型中
    this->states[stateNum] = newState;
}

void Model::setStartState(int stateNum) {
    State *state = this->states[stateNum];
    if (state == nullptr) {
        logger->error("未找到节点%d", stateNum);
        return;
    }
    if (hasStartState && state != startState) {
        logger->warning("已经指定过起始节点%d，将更新起始节点%d", startState->getStateNum(), state->getStateNum());
        startState->setStartFlag(false);
    } else {
        hasStartState = true;
    }
    startState = state;
    startState->setStartFlag(true);
    logger->info("节点%d成为了起始节点", startState->getStateNum());
}

void Model::setEndState(int stateNum) {
    State *state = this->states[stateNum];
    if (state == nullptr) {
        logger->error("未找到节点%d", stateNum);
        return;
    }
    if (hasEndState && state != endState) {
        logger->warning("已经指定过终止节点%d，将更新终止节点%d", endState->getStateNum(), state->getStateNum());
        endState->setEndFlag(false);
    } else {
        hasEndState = true;
    }
    endState = state;
    endState->setEndFlag(true);
    logger->info("节点%d成为了终止节点", endState->getStateNum());
}

void Model::addTran(const string &tranName, int sourceStateNum, int destStateNum) {
    // 如果源节点和目标节点尚不存在，则先创建相应状态节点
    State *sourceState = this->states[sourceStateNum];
    if (sourceState == nullptr) {
        sourceState = new State();
    }

    State *destState = this->states[destStateNum];
    if (destState == nullptr) {
        destState = new State();
    }

    // 创建Tran类，添加相应属性
    auto tran = new Tran();
    tran->setTranName(tranName);
    tran->setSourceState(sourceState);
    tran->setDestState(destState);

    // 将Tran类对象添加到源State和模型中
    sourceState->addTran(tran);
    this->trans.push_back(tran);
}

void Model::addSpec(const string &specStr) {
    // 生成Z3表达式添加进模型
    const Z3Expr z3Expr = this->extractZ3Expr(specStr, "");
    specZ3ExprVector.push_back(z3Expr);
}

bool Model::addEvent(const string &eventName, const map<string, string> &varValueMap) {
    logger->debug("当前节点为节点%d，开始尝试转移", currentState->getStateNum());

    // 如果当前是起始节点，则需要添加起始节点的表达式和SPEC表达式
    if (currentState == startState) {
        for (auto &z3Expr : startState->getZ3ExprList()) {
            this->slv.add(z3Expr);
            logger->debug("添加起始节点的表达式%s", z3Expr);;
        }
        for (auto &spec : specZ3ExprVector) {
            this->slv.add(spec);
            logger->debug("添加轨迹验证表达式%s", spec);
        }
        // 始终保留起始节点的表达式和SPEC表达式，后续添加的表达式在到达终止节点继续转移时弹出
        this->slv.push();

        // 初始化状态轨迹
        this->stateTrace.clear();
        this->stateTrace.push_back(startState);
    }

    // 如果当前是终止节点，则调整至起始节点开始转移
    if (currentState == endState) {
        currentState = startState;
        this->slv.pop();
        this->slv.push();

        // 重新初始化状态轨迹
        this->stateTrace.clear();
        this->stateTrace.push_back(startState);
        logger->info("从终止节点%d调整至起始节点%d开始转移", endState->getStateNum(),  startState->getStateNum());
    }

    // 初始化当前尝试失败的节点集合
    currentFailedStates.clear();

    // 假设不能转移
    bool result = false;
    const State *nextState = nullptr;

    // 先查看当前状态能否转移到相邻状态
    const vector<const Tran *> &outTranList = currentState->getTranList();
    for (const Tran *tran : outTranList) {
        nextState = tran->getDestState();
        if (tran->getTranName() == eventName) {
            result = this->verify(nextState, varValueMap);
            if (result) {
                break;
            }
        }
    }

    // 如果无法转移到相邻状态或者转移到相邻状态验证失败则在全局中搜索转移
    if (!result) {
        for (auto &tran : trans) {
            nextState = tran->getDestState();
            if (tran->getSourceState() != currentState && tran->getTranName() == eventName && currentFailedStates.find(nextState) == currentFailedStates.end()) {
                result = this->verify(nextState, varValueMap);
                if (result) {
                    break;
                }
            }
        }
    }

    if (result) {
        logger->info("事件\"%s\"导致节点%d转移到节点%d", eventName.c_str(), currentState->getStateNum(), nextState->getStateNum());
        currentState = const_cast<State *>(nextState);
        this->stateTrace.push_back(currentState);
        return true;
    } else {
        logger->warning("事件\"%s\"无法转移", eventName.c_str());
        return false;
    }
}

bool Model::initModel() {
    int num = 0;
    for (auto &kv : states) {
        if (kv.second->isEmpty()) {
            ++num;
        }
    }
    if (num > 0) {
        logger->error("模型中存在%d个虚拟的空节点", num);
        return false;
    }

    // 判断模型是否有起始和终止节点
    if (!hasStartState) {
        logger->error("模型未指定起始节点");
        return false;
    }
    if (!hasEndState) {
        logger->error("模型未指定终止节点");
        return false;
    }

    currentState = startState;
    return true;
}

const Z3Expr Model::extractZ3Expr(const string &exprStr, const string &serialNum) {
    // 借用运算符栈和运算数栈实现表达式的解析
    stack<string> operatorStack;
    // expr栈用来记录中间表达式结果
    stack<Z3Expr> exprStack;

    // 先压入最低优先级运算符
    operatorStack.push("$");

    // 遍历字符串表达式
    string identifier;    // 记录标识符
    string currentType;    // 记录当前类型
    for (auto c : exprStr) {
        if (currentType.empty()) {
            // 当前无类型，即可从任意符号开始新的标识符
            if (isalpha(c) || c == '_') {
                // 当前开始一个新的变量
                currentType = "var";
                identifier.push_back(c);
            } else if (isdigit(c) || c == '.') {
                // 当前开始一个新的运算数
                currentType = "operand";
                identifier.push_back(c);
            } else if (isOperator(c)) {
                // 当前为运算符
                currentType = "operator";
                identifier.push_back(c);
            } else if (!isspace(c)) {
                // 其他情况必须为空格
                logger->error("表达式中出现非法字符%c", c);
            }
        } else if (currentType == "var") {
            // 当前已处于变量状态
            if (isalnum(c) != 0 || c == '_') {
                // 继续当前变量
                identifier.push_back(c);
            } else {
                // 当前为其他字符则生成完整变量并考虑true或false的情况
                if (identifier == "true") {
                    exprStack.push(this->ctx.bool_val(true));
                }
                else if (identifier == "false") {
                    exprStack.push(this->ctx.bool_val(false));
                }
                else {
                    // 根据变量名生成变量名expr
                    exprStack.push(generateVarExp(identifier + serialNum));
                }
                identifier.clear();

                if (isspace(c) != 0) {
                    currentType.clear();
                } else if (isOperator(c)) {
                    currentType = "operator";
                    identifier.push_back(c);
                } else {
                    logger->error("表达式中出现不合法的标识符%s%c", identifier.c_str(), c);
                }
            }
        } else if (currentType == "operand") {
            // 当前已处于运算数状态
            if (isdigit(c) != 0 || c == '.') {
                // 继续当前运算数
                identifier.push_back(c);
            } else {
                // 当前为其他字符则生成完整运算数
                // 分析数的类型得到不同的expr
                exprStack.push(generateNumExp(identifier));
                identifier.clear();

                if (isspace(c) != 0) {
                    currentType = "";
                } else if (isOperator(c)) {
                    currentType = "operator";
                    identifier.push_back(c);
                } else {
                    logger->error("表达式中出现不合法的标识符%s%c", identifier.c_str(), c);
                }
            }
        } else if (currentType == "operator") {
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

                    Z3Expr expr2 = exprStack.top();
                    exprStack.pop();
                    Z3Expr expr1 = exprStack.top();
                    exprStack.pop();

                    exprStack.push(calcExpr(expr1, operatorTop, expr2));
                }
                // 退出循环时表示当前运算符比栈顶运算符优先级高
                operatorStack.push(identifier);
                identifier.clear();

                if (isalpha(c) != 0 || c == '_') {
                    currentType = "var";
                    identifier.push_back(c);
                } else if (isdigit(c) != 0 || c == '.') {
                    currentType = "operand";
                    identifier.push_back(c);
                } else if (isspace(c) != 0) {
                    currentType = "";
                } else {
                    logger->error("表达式中出现非法字符%c", c);
                }
            }
        }
    }

    // 循环结束后需要手动添加结束符, 即循环退栈
    if (currentType == "var") {
        // 根据变量名生成变量名expr
        exprStack.push(generateVarExp(identifier + serialNum));
        identifier.clear();
    } else if (currentType == "operand") {
        // 分析数的类型得到不同的expr
        exprStack.push(generateNumExp(identifier));
        identifier.clear();
    } else if (currentType == "operator") {
        logger->error("表达式\"%s\"以运算符结尾，非法！", exprStr.c_str());
    }

    while (operatorStack.size() > 1) {
        string operatorTop = operatorStack.top() + "";
        operatorStack.pop();

        if (exprStack.size() < 2) {
            logger->error("运算数或变量不足，表达式\"%s\"不合法", exprStr.c_str());
            break;
        }
        Z3Expr expr2 = exprStack.top();
        exprStack.pop();
        Z3Expr expr1 = exprStack.top();
        exprStack.pop();

        exprStack.push(calcExpr(expr1, operatorTop, expr2));
    }

    return exprStack.top();
}

const Z3Expr Model::generateVarExp(const string &varName) {
    auto digitIndex = varName.size();
    for (auto digitBegin = varName.rbegin(); digitBegin != varName.rend(); ++digitBegin, --digitIndex) {
        if (!isdigit(*digitBegin)) {
            break;
        }
    }

    string varNameWithoutNum = varName.substr(0, digitIndex);
    string serialNum = varName.substr(digitIndex);

    if (varsDecl.find(varNameWithoutNum) == varsDecl.end()) {
        logger->error("变量\"%s\"未定义", varName.c_str());
        return this->ctx.bool_val(true);
    }

    if (serialNum.empty() || states.find(std::stoi(serialNum)) == states.end()) {
        logger->error("变量\"%s\"缺少有效的序号后缀", varName.c_str());
        return this->ctx.bool_val(true);
    }

    string varType = varsDecl[varNameWithoutNum];
    if (varType == "int") {
        return this->ctx.int_const(varName.c_str());
    }
    else if (varType == "double") {
        return this->ctx.real_const(varName.c_str());
    }
    else if (varType == "bool") {
        return this->ctx.bool_const(varName.c_str());
    }
    else {
        logger->error("不支持的变量类型，变量为%s", varName.c_str());
        return this->ctx.bool_val(true);
    }
}

const Z3Expr Model::generateNumExp(const string &operand) {
    if (operand.find('.') != string::npos) return this->ctx.real_val(operand.c_str());
    else return this->ctx.int_val(operand.c_str());
}

bool Model::isOperator(char c) {
    static string operatorStr = "+-*/<>!=";
    return operatorStr.find(c) != string::npos;
}

bool Model::compareOperator(const string &operator1, const string &operator2) {
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
        logger->error("运算符\"%s\"不支持", operator1.c_str());
        return false;
    }
    if (operatorPriority.find(operator2) == operatorPriority.end()) {
        logger->error("运算符\"%s\"不支持", operator2.c_str());
        return false;
    }
    return operatorPriority[operator1] > operatorPriority[operator2];
}

const Z3Expr Model::calcExpr(const Z3Expr &expr1, const string &currentOperator, const Z3Expr &expr2) {
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

    logger->error("不支持的运算符\"%s\"", currentOperator.c_str());
    return expr1;
}

bool Model::verify(const State *nextState, const map<string, string> &varValueMap) {
    int nextStateNum = nextState->getStateNum();

    logger->debug("尝试转移到节点%d", nextStateNum);

    slv.push();
    // 先添加下一状态中的全部Z3表达式
    const vector<Z3Expr> &stateZ3ExprList = nextState->getZ3ExprList();
    for (auto &z3Expr : stateZ3ExprList) {
        slv.add(z3Expr);
        logger->debug("添加节点%d中的表达式%s", nextStateNum, z3Expr);
    }

    // 再将事件上的变量全部构造成Z3表达式添加进去
    for (auto &varValue : varValueMap) {
        string varType = varsDecl[varValue.first];
        if (varType.empty()) {
            logger->error("事件中出现未定义的变量%s", varValue.first.c_str());
            continue;
        }

        if (varType == "int") {
            Z3Expr z3Expr = this->ctx.int_const((varValue.first + std::to_string(nextStateNum)).c_str()) == this->ctx.int_val(varValue.second.c_str());
            slv.add(z3Expr);
            logger->debug("添加事件上变量值构成的表达式%s", z3Expr);
        } else if (varType == "double") {
            slv.add(this->ctx.real_const((varValue.first + std::to_string(nextStateNum)).c_str()) == this->ctx.real_val(varValue.second.c_str()));
        } else if (varType == "bool") {
            slv.add(this->ctx.bool_const((varValue.first + std::to_string(nextStateNum)).c_str()) == this->ctx.bool_val(varValue.second == "true"));
        }
    }

    if (slv.check() == z3::sat) {
        logger->info("尝试转移到节点%d成功", nextStateNum);
        return true;
    } else {
        logger->info("尝试转移到节点%d失败", nextStateNum);
        slv.pop();
        currentFailedStates.insert(nextState);
        return false;
    }
}
