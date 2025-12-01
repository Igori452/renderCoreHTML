#include "LayoutBox.h"

LayoutBox::LayoutBox(Node* node_) : 
    x(0.0), y(0.0), width(0.0), height(0.0), margins(0.0), parent(nullptr), node(node_) {}

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
}

void LayoutBox::setMargins(double margins_) {
    margins = margins_;
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

double LayoutBox::getMargins() const {
    return margins;
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