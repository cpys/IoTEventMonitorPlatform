//
// Created by chenkuan on 17-11-15.
//

#include <tinyxml2.h>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "StateParser.h"
using namespace tinyxml2;

StateParser::~StateParser() {
    delete model;
}

void StateParser::setStateXML(const string &stateXML) {
    this->stateXML = stateXML;
}

bool StateParser::parseStateXML() {
    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(stateXML.c_str());
    if (xmlError != XML_SUCCESS) {
        logger->error("无法解析状态机XML");
        logger->debug("状态机如下：%s", stateXML.c_str());
        return false;
    }

    XMLElement *mxGraphModel = xmlDocument.FirstChildElement("mxGraphModel");
    if (mxGraphModel == nullptr) {
        logger->error("state has no <mxGraphModel></xmGraphModel>!");
        return false;
    }

    XMLElement *root = mxGraphModel->FirstChildElement("root");
    if (root == nullptr) {
        logger->error("state has no <root></root>!");
        return false;
    }

    model = new Model();

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
        logger->error("缺少或非法的变量声明！");
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
#ifdef SHOW_RUN_TIME
    logger->timeBegin();
#endif

    XMLDocument xmlDocument;
    XMLError xmlError = xmlDocument.Parse(event.c_str());
    if (xmlError != XML_SUCCESS) {
        logger->error("event \"%s\"  不符合XML规范！", event.c_str());
        return false;
    }

    XMLElement *eventRoot = xmlDocument.FirstChildElement();
    auto eventName = eventRoot->Attribute("name");
    if (eventName == nullptr) {
        logger->error("event \"%s\"缺少事件名称！", event.c_str());
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

#ifdef SHOW_RUN_TIME
    logger->timeEnd();
    logger->showTime("验证事件总耗时");
#endif

    return result;
}

bool StateParser::parseVarDecl(const char *varDecl) {
    QString varDeclRawStr(varDecl);
    varDeclRawStr.replace("&amp;", "&");
    varDeclRawStr.replace("&lt;", "<");
    varDeclRawStr.replace("&gt;", ">");
    varDeclRawStr.replace("</div>", "<div>");
    varDeclRawStr.replace("<br>", "<div>");

    auto varDeclStrList = varDeclRawStr.split("<div>").toStdList();
    if (varDeclStrList.empty()) {
        return false;
    }

    for (auto& varDeclStr : varDeclStrList) {
        if (varDeclStr.isEmpty()) {
            continue;
        }
        auto varDeclSplitList = varDeclStr.split(":").toStdList();
        if (varDeclSplitList.size() != 2) {
            return false;
        }
        string varType = varDeclSplitList.back().trimmed().toStdString();
        string varName = varDeclSplitList.front().trimmed().toStdString();
        logger->info("添加变量声明：%s %s", varType.c_str(), varName.c_str());
        model->addVarDecl(varType, varName);
    }
    return true;
}

bool StateParser::parseState(const char *state, const char *stateGraphId, bool isEndState) {
    QString stateRawStr(state);
    stateRawStr.replace("&amp;", "&");
    stateRawStr.replace("&lt;", "<");
    stateRawStr.replace("&gt;", ">");
    stateRawStr.replace("</div>", "<div>");
    stateRawStr.replace("<br>", "<div>");

    auto strList = stateRawStr.split("<div>").toStdList();
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
        logger->error("state num is empty");
        return false;
    }
    int stateId = std::stoi(stateNumStr.toStdString());
    idMap[stoi(string(stateGraphId))] = stateId;

    string addStateLog = "添加状态";
    addStateLog.append(std::to_string(stateId));
    addStateLog.append(":");
    for (auto &stateConstraint : stateConstraints) {
        addStateLog.append(stateConstraint);
        addStateLog.append(",");
    }
    logger->info(addStateLog.c_str());

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
        logger->error("找不到转移 \"%s\" 的目的节点 %s", tran, target);
        return false;
    }
    int targetId = idMap[stoi(string(target))];

    if (source == nullptr) {
        model->setStartState(targetId);
        return true;
    }

    string tranName = tran;
    if (tranName.empty()) {
        logger->error("转移 \"%s\" 的转移名称为空", tran);
        return false;
    }
    if (idMap.find(stoi(string(source))) == idMap.end()) {
        logger->error("转移 \"%s\" 没有源节点 %s", tran, source);
        return false;
    }
    int sourceId = idMap[stoi(string(source))];

    logger->info("添加转移：%s %d-->%d", tranName.c_str(), sourceId, targetId);
    model->addTran(tranName, sourceId, targetId);
    return true;
}

bool StateParser::parseSpec(const char *spec) {
    QString specStr(spec);
    specStr.replace("&lt;", "<");
    specStr.replace("&gt;", ">");

    logger->info("添加验证:%s", specStr.toStdString().c_str());
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
        logger->error("event \"%s\" is not xml!", event.c_str());
        return false;
    }

    XMLElement *eventRoot = xmlDocument.FirstChildElement();
    auto eventName = eventRoot->Attribute("name");
    if (eventName == nullptr) {
        logger->error("缺少事件名称!");
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
