#ifndef LAYOUT_BOX_H
#define LAYOUT_BOX_H

#include <vector>

#include "../Node/Node.h"
#include "../Node/ElementNode.h"
#include "../Node/TextElement.h"
#include "../Style/Style.h"

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
        //геттеры для координат и размеров

        Node* getNode() const;
};

#endif