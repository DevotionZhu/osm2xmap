/*
 *    Copyright 2016 Semyon Yakimov
 *
 *    This file is part of Osm2xmap.
 *
 *    Osm2xmap is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Osm2xmap is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Osm2xmap.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "common.h"
#include "xml.h"

XmlElement::XmlElement(node_t * _node): node(_node) {
    name = getName();
}

bool 
XmlElement::operator==(const char* str) {
    return name == str;
}

std::string
XmlElement::getName() {
    if (node) {
        char * cName = roxml_get_name(node,nullptr,0);
        name = cName;
        roxml_release(cName);
    }
    return name;
}

XmlElement 
XmlElement::getChild(const std::string childName) const {
    return XmlElement(roxml_get_chld(node, (char*)childName.c_str(), 0));
}

XmlElement
XmlElement::getChild(int nb) const {
    return XmlElement(roxml_get_chld(node, nullptr, nb));
}

XmlElement 
XmlElement::getChild() const {
    return XmlElement(roxml_get_chld(node, nullptr, 0));
}

XmlElement&
XmlElement::operator++() {
    return (*this = XmlElement(roxml_get_next_sibling(node)));
}

class Attribute {
    char * value;
public:
    Attribute(node_t * node, const std::string& attrName) {
        node_t * attr = roxml_get_attr(node,(char*)attrName.c_str(),0);
        value = roxml_get_content(attr,nullptr,0,nullptr);
        if (value == nullptr) {
            throw Error("No attribute '" + attrName + "'");
        }
    };
    ~Attribute() {
        roxml_release(value);
    }
    const char * Value() const { return value; };
};

template < >
double
XmlElement::getAttribute(const std::string attrName) const {
    Attribute attribute(node,attrName);
    return atof(attribute.Value());
}

template < >
int
XmlElement::getAttribute(const std::string attrName) const {
    Attribute attribute(node,attrName);
    return atoi(attribute.Value());
}

template < >
long
XmlElement::getAttribute(const std::string attrName) const {
    if (node) {
        Attribute attribute(node,attrName);
        return atol(attribute.Value());
    }
    return 0;
}

template < >
std::string
XmlElement::getAttribute(const std::string attrName) const {
    node_t * attr = roxml_get_attr(node,(char*)attrName.c_str(),0);
    char * value = roxml_get_content(attr,nullptr,0,nullptr);
    std::string ret;
    if (value != nullptr) {
        ret = std::string(value);
    }
    roxml_release(value);
    return ret;
}

const std::string
XmlElement::getContent() const {
    const int crs_desc_len = 64;
    int size;
    char str[crs_desc_len];
    roxml_get_content(node, str, crs_desc_len, &size);
    return std::string(str);
}

int
XmlElement::getChildNumber() const {
    return roxml_get_chld_nb(node);
}

node_t *
XmlElement::addChildToBegin(const char * name) {
    return roxml_add_node(node, 1, ROXML_ELM_NODE, (char*)name, nullptr);
}

node_t *
XmlElement::addChildToEnd(const char * name) {
    return roxml_add_node(node, 0, ROXML_ELM_NODE, (char*)name, nullptr);
}

node_t * 
XmlElement::addChild(const char * name) {
    return addChildToEnd(name);
}

void
XmlElement::removeChild(const char * name) {
    node_t * chld = roxml_get_chld(node, (char*)name, 0);
    if (chld != nullptr) {
        roxml_del_node(chld);
    }
}

void
XmlElement::removeAttribute(const char * name) {
    node_t * attr = roxml_get_attr(node,(char*)name,0);
    if (attr != nullptr) {
        roxml_del_node(attr);
    }
}

void
XmlElement::addContent(const char * text) {
    roxml_add_node(node,0,ROXML_TXT_NODE,nullptr,(char*)text);
}

XmlTree::XmlTree(const char * inFilename) : XmlElement::XmlElement(roxml_load_doc((char*)inFilename)) {
    if (XmlElement::isEmpty()) {
        throw Error("opening XML file '" + std::string(inFilename) + "' failed");
    }
}

XmlTree::~XmlTree() {
    if (!XmlElement::isEmpty()) {
        roxml_close(node);
    }
}

void 
XmlTree::saveInFile(const char * outFilename, bool humanReadable) {
    roxml_commit_changes(node,(char*)outFilename,nullptr,humanReadable);
}

