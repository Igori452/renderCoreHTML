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

        void drawElement(LayoutBox& layoutBox) override;
        void drawText(LayoutBox& layoutBox) override;
    public:
        RendererSFML() = delete;
        RendererSFML(double windowWidth_, double windowHeight_);
        void renderLayoutTree(LayoutBox& layoutBox) override;
        void showScene(LayoutBox& rootLayoutBox);
        double computeTextWidth(const std::string& utf8Text, const TextMetrics& metrics) const override;
        double computeTextHeight(const std::string& utf8Text, const TextMetrics& metrics) const override;
};

#endif