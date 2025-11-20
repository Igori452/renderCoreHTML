#include "TextElement.h"

TextElement::TextElement() : Node(), text("") {}
TextElement::TextElement(const char* _text) : Node(), text(_text) {}
TextElement::TextElement(std::string _text) : Node(), text(_text) {}

std::unordered_map<StyleProperty, StyleValue> TextElement::getEffectiveTextStyles() const {
    std::unordered_map<StyleProperty, StyleValue> effectiveStyles;
    
    // Определяем текстовые свойства
    static const std::vector<StyleProperty> textRelatedProperties = {
        //StyleProperty::FONT_SIZE,
        //StyleProperty::FONT_FAMILY,
        //StyleProperty::FONT_WEIGHT,
        //StyleProperty::COLOR,
        //StyleProperty::LINE_HEIGHT,
        //StyleProperty::TEXT_ALIGN,
        //StyleProperty::LETTER_SPACING,
        //StyleProperty::WORD_SPACING
    };
    
    // Для каждого текстового свойства устанавливаем значение по умолчанию или из родителя
    for (const auto& prop : textRelatedProperties) {
        // Начинаем со значения по умолчанию
        StyleValue value = Style::getPropertiesDefault().at(prop);
        
        // Ищем переопределение в цепочке родителей (от текущего элемента к корню) при этом только у блочных элементов
        const Node* currentNode = this;

        while (currentNode != nullptr) {

            if (currentNode->getType() != Node::Type::TEXT_NODE) {
                ElementNode* elementNode = dynamic_cast<ElementNode*>(const_cast<Node*>(currentNode));
                const auto& nodeStyles = elementNode->getStyle().getMapProperty();
                
                auto it = nodeStyles.find(prop);
                if (it != nodeStyles.end()) {
                    value = it->second; // Нашли переопределение
                    break; // Ближайший родитель имеет приоритет
                }
            }

            currentNode = currentNode->getParent();
        }
        
        effectiveStyles[prop] = value;
    }
    
    return effectiveStyles;
}
        
void TextElement::applyMetrics() {
    auto effectiveStyles = getEffectiveTextStyles();
    
    // Применяем эффективные стили к метрикам
    auto colorIt = effectiveStyles.find(StyleProperty::COLOR);
    if (colorIt != effectiveStyles.end()) {
        uint32_t color = colorIt->second.getAs<double>().value();
        textMetrics.setColor(color);
    }
    
    // Можно добавить применение других стилей: font-weight, line-height и т.д.
}

const TextMetrics& TextElement::getTextMetrics() const {
    return textMetrics;
}

TextElement::Type TextElement::getType() const { 
    return Type::TEXT_NODE; 
}

const std::string TextElement::getText() const { 
    return text; 
}