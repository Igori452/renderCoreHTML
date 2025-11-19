#include "LayoutBox.h"

LayoutBox::LayoutBox(Node* node_) : 
    x(0.0), y(0.0), width(0.0), height(0.0), parent(nullptr), node(node_) {}

Node* LayoutBox::getNode() const { return node; }