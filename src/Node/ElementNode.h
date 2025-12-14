#ifndef ELEMENT_NODE_H
#define ELEMENT_NODE_H

#include <string>
#include <map>
#include <sstream>
#include "../Style/Style.h"
#include "Node.h"
#include "../Style/Style.h"
#include <algorithm>
#include <regex>

class ElementNode : public Node {
    private:
        std::map<std::string, std::string> attributes;
        Style style;
    public:
        ElementNode();
        ElementNode(std::string tagName);

        Type getType() const override;

        void setAttribute(std::string attrName, std::string attrValue);
        const std::map<std::string, std::string>& getAttributes() const;
        const Style& getStyle() const;
};

#endif