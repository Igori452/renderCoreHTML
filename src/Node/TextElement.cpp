#include "TextElement.h"

TextElement::TextElement() : Node(), text("") {}
TextElement::TextElement(const char* _text) : Node(), text(_text) {}
TextElement::TextElement(std::string _text) : Node(), text(_text) {}

std::unordered_map<StyleProperty, StyleValue> TextElement::getEffectiveTextStyles() const {
    std::unordered_map<StyleProperty, StyleValue> effectiveStyles;
    
    // Определяем текстовые свойства
    static const std::vector<StyleProperty> textRelatedProperties = {
        StyleProperty::FONT_SIZE,
        StyleProperty::FONT_FAMILY,
        StyleProperty::FONT_STYLE,
        StyleProperty::FONT_WEIGHT,
        StyleProperty::COLOR,
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
        uint32_t color = colorIt->second.getAs<uint32_t>().value();
        textMetrics.setColor(color);
    }

    auto fontIt = effectiveStyles.find(StyleProperty::FONT_FAMILY);
    if (fontIt != effectiveStyles.end()) {
        auto font = fontIt->second.getAs<std::string>().value();
        textMetrics.setFont(font);
    }

    auto fontStyleIt = effectiveStyles.find(StyleProperty::FONT_STYLE);
    if (fontStyleIt != effectiveStyles.end()) {
        auto fontStyle = fontStyleIt->second.getAs<StyleValue::FontStyleType>().value();
        textMetrics.setFontStyleType(fontStyle);
    }

    auto fontWeightIt = effectiveStyles.find(StyleProperty::FONT_WEIGHT);
    if (fontWeightIt != effectiveStyles.end()) {
        auto fontWeight = fontWeightIt->second.getAs<StyleValue::FontWeightType>().value();
        textMetrics.setFontWeightType(fontWeight);
    }

    auto fontSizeIt = effectiveStyles.find(StyleProperty::FONT_SIZE);
    if (fontSizeIt != effectiveStyles.end()) {
        auto fontSize = fontSizeIt->second.getAs<double>().value();
        textMetrics.setFontSize(fontSize);
    }
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