#include "LayoutBox.h"

LayoutBox::LayoutBox(Node* node_) : 
    x(0.0), y(0.0), width(0.0), height(0.0), marginX(0.0), marginY(0.0), 
    overflow(false), visibleWidth(0.0), visibleHeight(0.0), parent(nullptr), node(node_) {}

void LayoutBox::addChild(LayoutBox* layoutBox) {
    layoutBox->parent = this;
    children.push_back(layoutBox);
}

Node* LayoutBox::getNode() const { 
    return node; 
}

std::vector<LayoutBox*>& LayoutBox::getChildren() { 
    return children;
}

void LayoutBox::setPosition(double x_, double y_) {
    x = x_;
    y = y_;
}

void LayoutBox::setSize(double width_, double height_) {
    width = width_;
    height = height_;

    visibleWidth = width_;
    visibleHeight = height_;
}

void LayoutBox::setWidth(double width_) {
    width = width_;
    visibleWidth = width_;
}

void LayoutBox::setHeight(double height_) {
    height = height_;
    visibleHeight = height_;
}

void LayoutBox::setMarginX(double margins_) {
    marginX = margins_;
}

void LayoutBox::setMarginY(double margins_) {
    marginY = margins_;
}

void LayoutBox::setVisibleWidth(double visibleWidth_) {
    overflow = true;
    visibleWidth = visibleWidth_;
}   

void LayoutBox::setVisibleHeight(double visibleHeight_) {
    overflow = true;
    visibleHeight = visibleHeight_;
}

double LayoutBox::getX() const {
    return x;
}
double LayoutBox::getY() const {
    return y;
}

double LayoutBox::getWidth() const {
    return width;
}

double LayoutBox::getHeight() const {
    return height;
}

double LayoutBox::getMarginX() const {
    return marginX;
}

double LayoutBox::getMarginY() const {
    return marginY;
}

bool LayoutBox::isOverflow() const {
    return overflow;
}

double LayoutBox::getVisibleWidth() const {
    return visibleWidth;
}

double LayoutBox::getVisibleHeight() const {
    return visibleHeight;
}

void LayoutBox::printLayoutBoxTree() const {
    // Вспомогательная лямбда для рекурсивного вывода
    std::function<void(const LayoutBox*, int)> printRecursive;
    
    printRecursive = [&](const LayoutBox* box, int depth) {
        // Отступы
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        
        // Простые данные блока
        std::cout << "Box (" << int(box->getNode()->getType()) << "): (" << box->x << ", " << box->y << ") ";
        std::cout << box->width << "x" << box->height << "(" << box->getNode()->getTagName() << ")" << std::endl;
        
        // Дети
        for (const auto& child : box->children) {
            printRecursive(child, depth + 1);
        }
    };
    
    std::cout << "Layout Tree:" << std::endl;
    printRecursive(this, 0);
}