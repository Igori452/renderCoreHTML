#include "Node.h"
#include "ElementNode.h"
#include "TextElement.h"

Node::Node() : tagName("none"), parent(nullptr) {}
Node::Node(std::string _tagName) : tagName(_tagName), parent(nullptr) {}

Node::~Node() {
    for (Node* node : childrenNode) delete node;
}

std::string Node::getTagName() const {
    return tagName;
}

Node* Node::getParent() const {
    return parent;
}

void Node::addChild(Node* node) {
    node->parent = this;
    childrenNode.push_back(node);
}

void Node::setParent(Node* node) {
    parent = node;
}

const std::vector<Node*>& Node::getChildren() const {
    return childrenNode;
}

void Node::printNodes(int depth) {
    if (getType() == Type::ELEMENT_NODE) {
        std::string indent(depth * 2, ' ');
        
        // Открывающий тег
        std::cout << indent << "<" << tagName;
        
        // Атрибуты
        ElementNode* elementNode = dynamic_cast<ElementNode*>(this);
        if (elementNode) {
            for (const auto& pr : elementNode->getAttributes()) 
                std::cout << " " << pr.first << "=\"" << pr.second << "\"";
        }
        std::cout << ">" << std::endl;

        // Дети (рекурсия)
        for (Node* child : childrenNode)
            child->printNodes(depth + 1);
        
        // Закрывающий тег
        if (!childrenNode.empty()) {
            std::cout << std::endl << indent;
        }
        std::cout << "</" << tagName << ">";
        
    } else if (getType() == Type::TEXT_NODE) {
        TextElement* textNode = dynamic_cast<TextElement*>(this);
        if (textNode) {
            std::cout << textNode->getText();
        }
    }  
}
