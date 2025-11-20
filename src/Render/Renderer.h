#ifndef RENDERER_H
#define RENDERER_H

#include "../Layout/LayoutBox.h"

class Renderer {
    public:
        virtual void drawElement(LayoutBox& layoutBox) = 0;
        virtual void drawText(LayoutBox& layoutBox) = 0;
};

#endif