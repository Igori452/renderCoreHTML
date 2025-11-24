#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include "LayoutBox.h"

class LayoutEngine {
    private:
        const double windowWidth = 1280;
        const double windowHeight = 720;

        void computeRootElement(LayoutBox* rootBox, double availableWidth, double availableHeight);
        void computeBlockElement(LayoutBox* box, double parentX, double parentY, double availableWidth, double availableHeight);
        void computeInlineElement(LayoutBox* box, double& currentLineX, double& currentLineY, double& currentLineMaxHeight, double availableWidth);

    public:
        LayoutBox* buildLayoutTree(Node* rootNode);
        void computeLayout(LayoutBox* rootBox, double availableWidth, double availableHeight);
        
        double getWindowWidth() const;
        double getWindowHeight() const;

};

#endif