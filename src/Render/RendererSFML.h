#ifndef RENDERER_SFML_H
#define RENDERER_SFML_H

#include "Renderer.h"

#include "../Layout/LayoutBox.h"
#include <SFML/Graphics.hpp>

class RendererSFML : public Renderer {
    private:
        sf::RenderWindow window;
        float scrollOffset = 0.0f;
        float contentHeight = 0.0f;
        float windowHeight = 600.0f;

        void drawElement(LayoutBox& layoutBox) override;
        void drawText(LayoutBox& layoutBox) override;
    public:
        RendererSFML();
        void renderLayoutTree(LayoutBox& layoutBox) override;
        void showScene(LayoutBox& rootLayoutBox);
};

#endif