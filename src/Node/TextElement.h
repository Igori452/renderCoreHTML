#ifndef TEXT_ELEMENT_H
#define TEXT_ELEMENT_H

#include <string>
#include "Node.h"
#include "../Text/TextMetrics.h"
#include "ElementNode.h"

class TextElement : public Node {
    private:
        std::string text;
        TextMetrics textMetrics;

        // Метод для получения наследуемых стилей (снизу -> вверх)
        std::unordered_map<StyleProperty, StyleValue> getEffectiveTextStyles() const;
    public:
        TextElement();
        TextElement(const char* _text);
        TextElement(std::string _text);

        // Обновляет метрики на основе текущих эффективных стилей
        void applyMetrics();

        const TextMetrics& getTextMetrics() const;

        Type getType() const override;

        const std::string getText() const;
};

#endif