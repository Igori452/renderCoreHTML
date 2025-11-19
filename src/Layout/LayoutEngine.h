#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include "LayoutBox.h"

class LayoutEngine {
    private:
        LayoutEngine* layoutBox;
        const double windowWidth = 600;
        const double windowHeight = 800;

        void computeBlockELement(LayoutBox* box);
        void computeInlineELement(LayoutBox* box);

    public:
        LayoutBox* buildLayoutTree(Node* rootNode);
        void computeLayoutBox(LayoutBox* box, double parentX = 0, double parentY = 0);
};

#endif