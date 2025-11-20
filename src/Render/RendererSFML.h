#ifndef RENDERER_SFML_H
#define RENDERER_SFML_H

#include "Renderer.h"

class RendererSFML : public Renderer {
    public:
        void drawElement(LayoutBox& layoutBox) override;
        void drawText(LayoutBox& layoutBox) override;
};

#endif