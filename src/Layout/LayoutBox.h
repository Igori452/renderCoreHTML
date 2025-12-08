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
        double marginX, marginY;
        int dynamicBorderX, dynamicBorderY;

        // Добавление поддержки обрезания блоков (overflow)
        bool overflow;
        double visibleWidth, visibleHeight;

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

        void setMarginX(double margins_);
        void setMarginY(double margins_);

        void setSize(double width_, double height_);
        void setWidth(double width_);
        void setHeight(double height_);

        void setVisibleWidth(double visibleWidth_);
        void setVisibleHeight(double visibleHeight_);

        void setDynamicBorderX(int border_);
        void setDynamicBorderY(int border_);

        double getX() const;
        double getY() const;

        double getWidth() const;
        double getHeight() const;

        double getMarginX() const;
        double getMarginY() const;

        bool isOverflow() const;
        void setOverflow(bool overflow_);
    
        double getVisibleWidth() const;
        double getVisibleHeight() const;

        int getDynamicBorderX() const;
        int getDynamicBorderY() const;

        void printLayoutBoxTree() const;
};

#endif