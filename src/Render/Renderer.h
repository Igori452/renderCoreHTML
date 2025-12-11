#ifndef RENDERER_H
#define RENDERER_H

#include <string>

// Предварительное объявление
class TextMetrics;
class LayoutBox;

class Renderer {
    private:
        static const Renderer* activeRenderer;   // глобальная стратегия
        virtual void drawElement(LayoutBox& layoutBox) = 0;
        virtual void drawText(LayoutBox& layoutBox) = 0;
    public:
        virtual ~Renderer() = default;
        virtual void renderLayoutTree(LayoutBox& layoutBox) = 0;
        virtual double computeTextWidth(const std::string& utf8Text, const TextMetrics& metrics) const;

        static void init(const Renderer* r);
        static const Renderer* get();
};

#endif