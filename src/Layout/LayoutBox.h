#ifndef LAYOUT_BOX_H
#define LAYOUT_BOX_H

#include <vector>

#include "../Node/Node.h"
#include "../Node/ElementNode.h"
#include "../Node/TextElement.h"
#include "../Style/Style.h"
#include <iostream>
#include <functional>

class LayoutBox {
    private:
        double x, y;
        double width, height;

        LayoutBox* parent;
        std::vector<LayoutBox*> children;  
        Node* node;

        LayoutBox() = default;

    public:
        LayoutBox(Node* node_);
        
        void addChild(LayoutBox* layoutBox);

        Node* getNode() const;
        std::vector<LayoutBox*>& getChildren();

        void setPosition(double x_, double y_);
        void setSize(double width_, double height_);

        double getX() const;
        double getY() const;

        double getWidth() const;
        double getHeight() const;

        void printLayoutBoxTree() const;
};

#endif