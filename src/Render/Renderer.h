#ifndef RENDERER_H
#define RENDERER_H

#include "../Layout/LayoutBox.h"

class Renderer {
    private:
        virtual void drawElement(LayoutBox& layoutBox) = 0;
        virtual void drawText(LayoutBox& layoutBox) = 0;
    public:
        virtual void renderLayoutTree(LayoutBox& layoutBox) = 0;
};

#endif