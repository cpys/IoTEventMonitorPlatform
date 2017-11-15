//
// Created by chenkuan on 17-11-15.
//

#include <DFAModule.h>
#include <tinyxml2.h>
#include <iostream>
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

bool StateParser::parseState() {
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

    for (XMLElement *mxCell = root->FirstChildElement("mxCell"); mxCell != nullptr; mxCell = mxCell->NextSiblingElement("mxCell")) {
        auto edgeAttribute = mxCell->Attribute("edge");
        if (edgeAttribute != nullptr && strcmp("1", edgeAttribute) == 0) {
            cout << mxCell->Attribute("id") << " is edge, ";
            auto source = mxCell->Attribute("source");
            if (source != nullptr) {
                cout << "source = " << source << endl;
            }
            auto target = mxCell->Attribute("target");
            if (target != nullptr) {
                cout << "target = " << target << endl;
            }
            continue;
        }

        auto style = mxCell->Attribute("style");
        if (style == nullptr) continue;
        if (strncmp("ellipse", style, 7) == 0) {
            cout << mxCell->Attribute("id") << " is state" << endl;
            continue;
        }

        if (strncmp("shape=process", style, 13) == 0) {
            cout << mxCell->Attribute("id") << " is declaration" << endl;
            continue;
        }
    }

    for (XMLElement *userObject = root->FirstChildElement("UserObject"); userObject != nullptr; userObject = userObject->NextSiblingElement("UserObject")) {
        cout << userObject->Attribute("id") << " is spec" << endl;
    }


    return false;
}

Module *StateParser::getModule() {
    return module;
}

bool StateParser::validateEvent(const string &event) {
    return false;
}
