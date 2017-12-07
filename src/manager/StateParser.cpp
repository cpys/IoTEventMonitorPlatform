//
// Created by chenkuan on 17-11-15.
//

#include <tinyxml2.h>
#include <iostream>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "StateParser.h"
using namespace tinyxml2;
using std::cout;
using std::cerr;
using std::endl;

StateParser::StateParser() {
    ofile.open("/home/chenkuan/timelog.txt");
}

StateParser::~StateParser() {
    delete model;
    ofile.close();
}

void StateParser::setStateXML(const string &stateXML) {
    this->stateXML = stateXML;
}

bool StateParser::parseStateXML() {
    model = new Model();

    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(stateXML.c_str());
    if (xmlError != XML_SUCCESS) {
        cerr << "Cannot parse state xml!" << endl;
        cerr << "状态机如下：" << endl;
        cerr << stateXML.c_str() << endl;
        return false;
    }

    XMLElement *mxGraphModel = xmlDocument.FirstChildElement("mxGraphModel");
    if (mxGraphModel == nullptr) {
        cerr << "state has no <mxGraphModel></xmGraphModel>!" << endl;
        return false;
    }

    XMLElement *root = mxGraphModel->FirstChildElement("root");
    if (root == nullptr) {
        cerr << "state has no <root></root>!" << endl;
        return false;
    }

    // 添加变量声明
    bool hasVarDecl = false;
    for (auto mxCell = root->FirstChildElement("mxCell"); mxCell != nullptr; mxCell = mxCell->NextSiblingElement("mxCell")) {
        auto style = mxCell->Attribute("style");
        if (style != nullptr && strncmp("shape=process", style, 13) == 0) {
            if (parseVarDecl(mxCell->Attribute("value"))) {
                hasVarDecl = true;
            }
            break;
        }
    }
    if (!hasVarDecl) {
        cerr << "缺少或非法的变量声明！" << endl;
        return false;
    }

    // 添加状态节点
    for (auto mxCell = root->FirstChildElement("mxCell"); mxCell != nullptr; mxCell = mxCell->NextSiblingElement("mxCell")) {
        auto style = mxCell->Attribute("style");
        if (style != nullptr && strncmp("ellipse", style, 7) == 0) {
            if (strncmp("ellipse;shape=doubleEllipse", style, 27) == 0) {
                if (!parseState(mxCell->Attribute("value"), mxCell->Attribute("id"), true)) {
                    return false;
                }
            }
            else if (!parseState(mxCell->Attribute("value"), mxCell->Attribute("id"), false)) {
                return false;
            }
        }
    }

    // 添加转移
    for (auto mxCell = root->FirstChildElement("mxCell"); mxCell != nullptr; mxCell = mxCell->NextSiblingElement("mxCell")) {
        auto edgeAttribute = mxCell->Attribute("edge");
        auto source = mxCell->Attribute("source");
        // 如果没有该属性是不是返回空指针
        auto target = mxCell->Attribute("target");
        if (edgeAttribute != nullptr && strcmp("1", edgeAttribute) == 0 && target != nullptr) {
            if (!parseTran(mxCell->Attribute("value"), source, target)) {
                return false;
            }
        }
    }

    model->initModel();

    // 添加验证
    for (auto userObject = root->FirstChildElement("UserObject"); userObject != nullptr; userObject = userObject->NextSiblingElement("UserObject")) {
        if (!parseSpec(userObject->Attribute("label"))) {
            return false;
        }
    }

    return true;
}

bool StateParser::validateEvent(const string &event) {
    clock_t startTime, endTime;
    startTime = clock();

    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(event.c_str());
    if (xmlError != XML_SUCCESS) {
        cerr << "event \"" << event << "\"  符合XML规范！" << endl;
        return false;
    }

    XMLElement *eventRoot = xmlDocument.FirstChildElement();
    auto eventName = eventRoot->Attribute("name");
    if (eventName == nullptr) {
        cerr << "缺少事件名称!" << endl;
        return false;
    }
    auto eventImportant = eventRoot->Attribute("important");
    if (eventImportant == nullptr || strcmp("1", eventImportant) != 0) {
        isEventImportant = false;
    }
    else {
        isEventImportant = true;
    }

    map<string, string> vars;
    for (auto varLabel = eventRoot->FirstChildElement(); varLabel != nullptr; varLabel = varLabel->NextSiblingElement()) {
        vars[string(varLabel->Value())] = string(varLabel->GetText());
    }

    bool result = model->addEvent(string(eventName), vars);

    endTime = clock();
    cout << "  验证事件总耗时 " << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    ofile << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << endl;

    return result;
}

bool StateParser::parseVarDecl(const char *varDecl) {
    QString varDeclStr(varDecl);
    varDeclStr.replace("&amp;", "&");
    varDeclStr.replace("&lt;", "<");
    varDeclStr.replace("&gt;", ">");
    varDeclStr.replace("</div>", "<div>");
    varDeclStr.replace("<br>", "<div>");

    auto varDeclStrList = varDeclStr.split("<div>").toStdList();
    if (varDeclStrList.empty()) {
        return false;
    }

    for (auto& varDecl : varDeclStrList) {
        if (varDecl.isEmpty()) {
            continue;
        }
        auto varDeclSplitList = varDecl.split(":").toStdList();
        if (varDeclSplitList.size() != 2) {
            return false;
        }
        cout << "添加变量声明：" << varDeclSplitList.back().trimmed().toStdString() << " : " << varDeclSplitList.front().trimmed().toStdString() << endl;
        model->addVarDecl(varDeclSplitList.back().trimmed().toStdString(), varDeclSplitList.front().trimmed().toStdString());
    }
    return true;
}

bool StateParser::parseState(const char *state, const char *stateGraphId, bool isEndState) {
    QString stateStr(state);
    stateStr.replace("&amp;", "&");
    stateStr.replace("&lt;", "<");
    stateStr.replace("&gt;", ">");
    stateStr.replace("</div>", "<div>");
    stateStr.replace("<br>", "<div>");

    auto strList = stateStr.split("<div>").toStdList();
    if (strList.empty()) {
        return false;
    }
    vector<string> stateConstraints;

    bool isFirstLine = true;
    QString stateNumStr;
    for (auto &str : strList) {
        if (str.isEmpty()) {
            continue;
        }
        if (isFirstLine) {
            isFirstLine = false;
            stateNumStr = str;
            continue;
        }
        stateConstraints.push_back(str.trimmed().toStdString());
    }

    if (stateNumStr.isEmpty()) {
        cerr << "state num is empty" << endl;
        return false;
    }
    int stateId = std::stoi(stateNumStr.toStdString());
    idMap[stoi(string(stateGraphId))] = stateId;

    cout << "添加状态" << stateId << ":";
    for (auto stateConstraint : stateConstraints) {
        cout << stateConstraint << ",";
    }
    cout << endl;
    if (isEndState) {
        model->addEndState(stateId, stateConstraints);
    }
    else {
        model->addState(stateId, stateConstraints);
    }
    return true;
}

bool StateParser::parseTran(const char *tran, const char *source, const char *target) {
    if (idMap.find(stoi(string(target))) == idMap.end()) {
        cerr << "没有目的节点" << target << endl;
        return false;
    }
    int targetId = idMap[stoi(string(target))];

    if (source == nullptr) {
        model->setStartState(targetId);
        return true;
    }

    string tranName = tran;
    if (tranName.empty()) {
        cerr << "tranName is empty" << endl;
        return false;
    }
    if (idMap.find(stoi(string(source))) == idMap.end()) {
        cerr << "没有源节点" << source << endl;
        return false;
    }
    int sourceId = idMap[stoi(string(source))];

    cout << "添加转移：" << tranName << " " << sourceId << "-->" << targetId << endl;
    model->addTran(tranName, sourceId, targetId);
    return true;
}

bool StateParser::parseSpec(const char *spec) {
    QString specStr(spec);
    specStr.replace("&lt;", "<");
    specStr.replace("&gt;", ">");

    cout << "添加验证:" << specStr.toStdString() << endl;
    model->addSpec({specStr.toStdString()});
    return true;
}

bool StateParser::getIsEventImportant() {
    return isEventImportant;
}

bool StateParser::justGetIsEventImportant(const string &event) {
    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(event.c_str());
    if (xmlError != XML_SUCCESS) {
        cerr << "event \"" << event << "\" is not xml!" << endl;
        return false;
    }

    XMLElement *eventRoot = xmlDocument.FirstChildElement();
    auto eventName = eventRoot->Attribute("name");
    if (eventName == nullptr) {
        cerr << "缺少事件名称!" << endl;
        return false;
    }
    auto eventImportant = eventRoot->Attribute("important");
    if (eventImportant == nullptr || strcmp("1", eventImportant) != 0) {
        return false;
    }
    else {
        return true;
    }
}
