//
// Created by chenkuan on 17-11-15.
//

#include <DFAModule.h>
#include <tinyxml2.h>
#include <iostream>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "StateParser.h"
using namespace tinyxml2;
using std::cerr;
using std::endl;

StateParser::~StateParser() {
    delete module;
}

void StateParser::setStateXML(const string &stateXML) {
    this->stateXML = stateXML;
}

bool StateParser::parseStateXML() {
    module = new DFAModule();

    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(stateXML.c_str());
    if (xmlError != XML_SUCCESS) {
        cerr << "Cannot parse state xml!" << endl;
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
            if (!parseState(mxCell->Attribute("value"))) {
                return false;
            }
        }
    }

    // 添加转移
    for (auto mxCell = root->FirstChildElement("mxCell"); mxCell != nullptr; mxCell = mxCell->NextSiblingElement("mxCell")) {
        auto edgeAttribute = mxCell->Attribute("edge");
        auto source = mxCell->Attribute("source");
        auto target = mxCell->Attribute("target");
        if (edgeAttribute != nullptr && strcmp("1", edgeAttribute) == 0 && source != nullptr && target != nullptr) {
            if (!parseTran(mxCell->Attribute("value"), source, target)) {
                return false;
            }
        }
    }

    module->initModule();

    // 添加验证
    for (auto userObject = root->FirstChildElement("UserObject"); userObject != nullptr; userObject = userObject->NextSiblingElement("UserObject")) {
        if (!parseSpec(userObject->Attribute("label"))) {
            return false;
        }
    }

    return true;
}

bool StateParser::validateEvent(const string &event) {
    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(event.c_str());
    if (xmlError != XML_SUCCESS) {
        cerr << "event \"" << event << "\" is not xml!" << endl;
        return true;
    }

    XMLElement *eventRoot = xmlDocument.FirstChildElement();
    auto eventName = eventRoot->Attribute("name");
    if (eventName == nullptr) {
        cerr << "缺少事件名称!" << endl;
        return true;
    }

    map<string, string> vars;
    for (auto varLabel = eventRoot->FirstChildElement(); varLabel != nullptr; varLabel = varLabel->NextSiblingElement()) {
        vars[string(varLabel->Value())] = string(varLabel->GetText());
    }

    return module->addEvent(string(eventName), vars);
}

bool StateParser::parseVarDecl(const char *varDecl) {
    auto varDeclStrList = QString(varDecl).split("&lt;br&gt;").toStdList();
    for (auto& varDeclStr : varDeclStrList) {
        auto varDeclSplitList = varDeclStr.split(":").toStdList();
        if (varDeclSplitList.size() != 2) {
            return false;
        }
        module->addVarDecl(varDeclSplitList.back().trimmed().toStdString(), varDeclSplitList.front().trimmed().toStdString());
    }
    return true;
}

bool StateParser::parseState(const char *state) {
    QString stateStr(state);
    stateStr.replace("&amp;", "&");
    stateStr.replace("&lt;", "<");
    stateStr.replace("&gt;", ">");
    stateStr.replace("</div>", "<div>");

    auto strList = stateStr.split("<div>").toStdList();
    if (strList.size() <= 0) {
        return false;
    }
    vector<string> statConstraints;

    bool isFirstLine = true;
    for (auto &str : strList) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (str.isEmpty()) continue;
        statConstraints.push_back(str.trimmed().toStdString());
    }

    module->addState(std::stoi(strList.front().toStdString()), statConstraints);
    return true;
}

bool StateParser::parseTran(const char *tran, const char *source, const char *target) {
    // TODO
    return true;
}

bool StateParser::parseSpec(const char *spec) {
    // TODO
    return false;
}
