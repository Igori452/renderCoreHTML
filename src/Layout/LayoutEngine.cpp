#include "LayoutEngine.h"

LayoutBox* LayoutEngine::buildLayoutTree(Node* rootNode) {
    LayoutBox* rootBox = new LayoutBox(rootNode);

    // можно использовать двухвазный проход для поодержки auto у height и width

    return rootBox;
}

void LayoutEngine::computeLayoutBox(LayoutBox* box, double parentX = 0, double parentY = 0) {
    Node* node = box->getNode();

    switch (node->getType())
    {
        // Обработка тегов
        case Node::Type::ELEMENT_NODE: {
            ElementNode* elementNode = dynamic_cast<ElementNode*>(node);
            const std::unordered_map<StyleProperty, StyleValue> styleElement = elementNode->getStyle().getMapProperty();

            StyleValue::DisplayType display = styleElement.find(StyleProperty::DISPLAY)->second.getAs<StyleValue::DisplayType>().value(); 

            if (display == StyleValue::DisplayType::BLOCK) computeBlockELement(box);
            
            else if (display == StyleValue::DisplayType::INLINE) computeBlockELement(box);
            
            break;
        }
        
        // Обработка текста всегде (обрабатывается как inline)
        case Node::Type::TEXT_NODE: {
            TextElement* textELement = dynamic_cast<TextElement*>(node);
            computeBlockELement(box);

            break;
        }
    }
}

void computeBlockELement(LayoutBox* box) {

}

void computeInlineELement(LayoutBox* box) {
    
}