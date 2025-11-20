#include "LayoutEngine.h"

LayoutBox* LayoutEngine::buildLayoutTree(Node* rootNode) {
    LayoutBox* rootBox = new LayoutBox(rootNode);

    // Рекурсивно строим дерево для детей
    for (const auto child : rootNode->getChildren()) {
        LayoutBox* childBox = buildLayoutTree(child);
        rootBox->addChild(childBox);
    }

    return rootBox;
}

double LayoutEngine::getWindowWidth() const {
    return windowWidth;
}

double LayoutEngine::getWindowHeight() const {
    return windowHeight;
}

void LayoutEngine::computeLayout(LayoutBox* rootBox, double availableWidth, double availableHeight) {
    computeRootElement(rootBox, availableWidth, availableHeight);
}

void LayoutEngine::computeRootElement(LayoutBox* rootBox, double availableWidth, double availableHeight) {
    Node* rootNode = rootBox->getNode();
    
    // Корневой элемент всегда занимает всю доступную область
    rootBox->setPosition(0, 0);
    rootBox->setSize(availableWidth, availableHeight);
    
    // Обрабатываем детей корневого элемента
    double childY = 0;
    double childX = 0;
    
    for (auto child : rootBox->getChildren()) {
        Node* childNode = child->getNode();
        ElementNode* elementNode = dynamic_cast<ElementNode*>(childNode);
        
        if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::BLOCK) {
            computeBlockElement(child, childX, childY, availableWidth, availableHeight - childY);
            childY += child->getHeight();
        } else {
            double lineY = childY;
            double lineMaxHeight = 0;
            computeInlineElement(child, childX, lineY, lineMaxHeight, availableWidth);
            childY = lineY + lineMaxHeight;
        }
    }
}

void LayoutEngine::computeBlockElement(LayoutBox* box, double parentX, double parentY, double availableWidth, double availableHeight) {
    Node* node = box->getNode();
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node);

    if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::BLOCK) {
        
        // 1. Получаем отступы и margins
        double marginTop = elementNode->getStyle().getProperty(StyleProperty::MARGIN_TOP).getAs<double>().value();
        double marginRight = elementNode->getStyle().getProperty(StyleProperty::MARGIN_RIGHT).getAs<double>().value();
        double marginBottom = elementNode->getStyle().getProperty(StyleProperty::MARGIN_BOTTOM).getAs<double>().value();
        double marginLeft = elementNode->getStyle().getProperty(StyleProperty::MARGIN_LEFT).getAs<double>().value();
        
        double paddingLeft = elementNode->getStyle().getProperty(StyleProperty::PADDING_LEFT).getAs<double>().value();
        double paddingRight = elementNode->getStyle().getProperty(StyleProperty::PADDING_RIGHT).getAs<double>().value();
        double paddingTop = elementNode->getStyle().getProperty(StyleProperty::PADDING_TOP).getAs<double>().value();
        double paddingBottom = elementNode->getStyle().getProperty(StyleProperty::PADDING_BOTTOM).getAs<double>().value();
        
        // 2. Вычисляем ширину блока
        double contentWidth = availableWidth - marginLeft - marginRight - paddingLeft - paddingRight;

        // 3. Позиционируем блок
        double x = parentX + marginLeft;
        double y = parentY + marginTop;
        
        box->setPosition(x, y);
        
        // 4. Обрабатываем детей
        double childY = y + paddingTop;
        double childX = x + paddingLeft;

        // Для inline-элементов внутри блока
        double lineX = childX;
        double lineY = childY;
        double lineMaxHeight = 0;

        for (auto child : box->getChildren()) {
            Node* childNode = child->getNode();
            ElementNode* childElementNode = dynamic_cast<ElementNode*>(childNode);
            
            bool isBlockChild = false;
            if (childElementNode != nullptr) {
                // Проверяем display тип ребенка
                auto displayValue = childElementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value();
                isBlockChild = (displayValue == StyleValue::DisplayType::BLOCK);
            }
            
            if (isBlockChild) {
                // Блочный ребенок
                computeBlockElement(child, childX, childY, contentWidth, availableHeight - childY);
                childY += child->getHeight();
                
                // Сбрасываем inline состояние после блочного элемента
                lineX = childX;
                lineY = childY;
                lineMaxHeight = 0;
                
            } else {
                // Inline ребенок (элемент или текстовый узел)
                computeInlineElement(child, lineX, lineY, lineMaxHeight, contentWidth);
                
                // После обработки inline элементов обновляем позицию Y
                childY = lineY + lineMaxHeight;
                lineX = childX; // Сброс для следующей строки
                lineY = childY;
                lineMaxHeight = 0;
            }
        }

        // Проверяем явно заданную ширину и задаем относительно детей если ее нету
        double explicitWidth = elementNode->getStyle().getProperty(StyleProperty::WIDTH).getAs<double>().value();
        if (explicitWidth > 0) contentWidth = std::min(explicitWidth, contentWidth);
        else {
            double maxChildWidth = 0;
            for (auto child : box->getChildren()) {
                maxChildWidth = std::max(maxChildWidth, child->getWidth());
            }
            if (maxChildWidth > 0) {
                contentWidth = std::min(maxChildWidth, contentWidth);
            }
        }

        // 5. Вычисляем высоту блока
        double contentHeight = childY - y + paddingBottom;

        double explicitHeight = elementNode->getStyle().getProperty(StyleProperty::HEIGHT).getAs<double>().value();
        if (explicitHeight > 0) {
            contentHeight = explicitHeight;
        }

        // 6. Устанавливаем финальный размер
        double totalWidth = contentWidth + paddingLeft + paddingRight;
        double totalHeight = contentHeight + paddingTop + paddingBottom;

        box->setSize(totalWidth, totalHeight);
    }
}

void LayoutEngine::computeInlineElement(LayoutBox* box, double& currentLineX, double& currentLineY, double& currentLineMaxHeight, double availableWidth) {
    // currentLineX - текущая позиция x в строке, currentLineY - текущая позиция y в строке, currentLineMaxHeight - максимальная высота строки, availableWidth - доступная ширина для размещения
    
    Node* node = box->getNode();
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node);

    if ((elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::INLINE)
        || node->getType() == Node::Type::TEXT_NODE) {
        
        double contentWidth = 0;
        double contentHeight = 0;
        
        if (node->getType() == Node::Type::TEXT_NODE) {
            // Текстовый узел - используем готовые метрики
            TextElement* textElement = dynamic_cast<TextElement*>(node);
            contentWidth = textElement->getTextMetrics().getTextWidth(textElement->getText());
            contentHeight = textElement->getTextMetrics().getTextHeight(textElement->getText());
        } else {
            // Дети начинают с ТЕКУЩЕЙ позиции в строке, а не с 0
            double childLineX = currentLineX;
            double childLineY = currentLineY;
            double childLineMaxHeight = currentLineMaxHeight;
            
            for (auto child : box->getChildren()) {
                computeInlineElement(child, childLineX, childLineY, childLineMaxHeight, availableWidth);
                LayoutBox* childBox = dynamic_cast<LayoutBox*>(child);
            }
            
            // ИЗМЕНЕНИЕ: Вычисляем общую ширину как разницу между конечной и начальной позицией
            contentWidth = childLineX - currentLineX;
            contentHeight = childLineMaxHeight;
            
            // Учитываем padding
            double paddingLeft = elementNode->getStyle().getProperty(StyleProperty::PADDING_LEFT).getAs<double>().value();
            double paddingRight = elementNode->getStyle().getProperty(StyleProperty::PADDING_RIGHT).getAs<double>().value();
            double paddingTop = elementNode->getStyle().getProperty(StyleProperty::PADDING_TOP).getAs<double>().value();
            double paddingBottom = elementNode->getStyle().getProperty(StyleProperty::PADDING_BOTTOM).getAs<double>().value();
            
            contentWidth += paddingLeft + paddingRight;
            contentHeight += paddingTop + paddingBottom;
        }
        
        // Проверяем, помещается ли элемент в текущую строку
        if (currentLineX + contentWidth > availableWidth && currentLineX > 0) {
            // Перенос на новую строку
            currentLineX = 0;
            currentLineY += currentLineMaxHeight;
            currentLineMaxHeight = 0;
        }
        
        // Устанавливаем позицию и размер
        box->setPosition(currentLineX, currentLineY);
        box->setSize(contentWidth, contentHeight);
        
        // Обновляем состояние строки для следующего элемента (так как работа идет через ссылку)
        currentLineX += contentWidth;
        currentLineMaxHeight = std::max(currentLineMaxHeight, contentHeight);
        
        // Обработка inline-block элементов
        if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::INLINE_BLOCK) {
            // Inline-block ведет себя как block внутри, но как inline снаружи
            // После inline-block принудительный перенос строки
            currentLineX = 0;
            currentLineY += currentLineMaxHeight;
            currentLineMaxHeight = 0;
        }
    }
}