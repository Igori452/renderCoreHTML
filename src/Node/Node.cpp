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