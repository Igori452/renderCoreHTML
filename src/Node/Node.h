#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>
#include "../Style/Style.h"

class Node {
    public:
        enum class Type { ELEMENT_NODE, TEXT_NODE };
    private:
        std::string tagName;
        std::vector<Node*> childrenNode;
        Node* parent;
    public:
        Node();
        Node(std::string tagName);
        virtual ~Node();

        std::string getTagName() const;
        Node* getParent() const;

        void addChild(Node* node);
        const std::vector<Node*>& getChildren() const;

        void printNodes(int depth = 0);

        Type virtual getType() const = 0;

        //virtual void render (Renderer & renderer) = 0;
};

#endif