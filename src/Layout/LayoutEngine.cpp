#include "LayoutEngine.h"

LayoutBox* LayoutEngine::buildLayoutTree(Node* rootNode) {
    ElementNode* elementNode = dynamic_cast<ElementNode*>(rootNode);
    
    // Если элемент имеет display: none, не создаем для него LayoutBox
    if (elementNode != nullptr && 
        elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::NONE) {
        return nullptr;
    }
    
    LayoutBox* rootBox = new LayoutBox(rootNode);

    // Рекурсивно строим дерево для детей
    for (const auto child : rootNode->getChildren()) {
        LayoutBox* childBox = buildLayoutTree(child);
        if (childBox != nullptr) {
            rootBox->addChild(childBox);
        }
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
    
    // Обрабатываем детей корневого элемента
    double childY = 0;
    double childX = 0;
    double maxHeightContent = 0;

    for (auto child : rootBox->getChildren()) {
        Node* childNode = child->getNode();
        ElementNode* elementNode = dynamic_cast<ElementNode*>(childNode);
        
        if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::BLOCK) {
            computeBlockElement(child, childX, childY, availableWidth, availableHeight);
            childY += child->getHeight();
            maxHeightContent = std::max(maxHeightContent, availableHeight);
        } else {
            double lineY = childY;
            double lineMaxHeight = 0;
            computeInlineElement(child, childX, lineY, lineMaxHeight, availableWidth);
            childY = lineY + lineMaxHeight;
        }
    }

    rootBox->setSize(availableWidth, maxHeightContent);
}

void LayoutEngine::computeBlockElement(LayoutBox* box, double parentX, double parentY, double availableWidth, double& availableHeight) {
    Node* node = box->getNode();
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node);

    if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::BLOCK) {
        
        // 1. Получаем отступы, margins, borderы
        double marginTop = elementNode->getStyle().getProperty(StyleProperty::MARGIN_TOP).getAs<double>().value();
        double marginRight = elementNode->getStyle().getProperty(StyleProperty::MARGIN_RIGHT).getAs<double>().value();
        double marginBottom = elementNode->getStyle().getProperty(StyleProperty::MARGIN_BOTTOM).getAs<double>().value();
        double marginLeft = elementNode->getStyle().getProperty(StyleProperty::MARGIN_LEFT).getAs<double>().value();
        
        double paddingLeft = elementNode->getStyle().getProperty(StyleProperty::PADDING_LEFT).getAs<double>().value();
        double paddingRight = elementNode->getStyle().getProperty(StyleProperty::PADDING_RIGHT).getAs<double>().value();
        double paddingTop = elementNode->getStyle().getProperty(StyleProperty::PADDING_TOP).getAs<double>().value();
        double paddingBottom = elementNode->getStyle().getProperty(StyleProperty::PADDING_BOTTOM).getAs<double>().value();
        
        double border = elementNode->getStyle().getProperty(StyleProperty::BORDER_WIDTH).getAs<double>().value();

        // 2. Вычисляем ширину блока
        double contentWidth = availableWidth - marginLeft - marginRight - paddingLeft - paddingRight;

        // 3. Позиционируем блок
        double x = parentX + marginLeft + border;
        double y = parentY + marginTop + border;
        
        box->setPosition(x, y);
        
        // 4. Обрабатываем детей
        double childY = y + paddingTop;
        double childX = x + paddingLeft;

        // Для inline-элементов внутри блока
        double lineX = childX;
        double lineY = childY;
        double lineMaxHeight = 0;

        double offsetY = childY;
        for (auto child : box->getChildren()) {
            Node* childNode = child->getNode();
            ElementNode* childElementNode = dynamic_cast<ElementNode*>(childNode);
            
            bool isBlockChild = false;
            if (childElementNode != nullptr) {
                // Проверяем display и тип ребенка
                auto displayValue = childElementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value();
                isBlockChild = (displayValue == StyleValue::DisplayType::BLOCK);
            }
            
            if (isBlockChild) {
                double childeHeight = availableHeight - childY;

                std::cout << "ChildY: " << childY << ' ';

                computeBlockElement(child, childX, offsetY, contentWidth, childeHeight);
                offsetY += childeHeight;

                std::cout  << "offsetY: " << offsetY << std::endl;

                // Сбрасываем inline состояние
                lineX = childX;
                lineY = childY;
                lineMaxHeight = 0;
            } else {
                // Inline ребенок (элемент или текстовый узел)
                computeInlineElement(child, lineX, lineY, lineMaxHeight, contentWidth);
                offsetY += lineMaxHeight;

                // После обработки inline элементов обновляем позицию Y
                childY += lineMaxHeight;
                lineY = childY;
                lineMaxHeight = 0;
            }
        }

        // 5. Вычисляем высоту блока
        double contentHeight = offsetY;

         // Проверяем явно заданную ширину и высоту и задаем относительно детей если ее нету
        double explicitHeight = elementNode->getStyle().getProperty(StyleProperty::HEIGHT).getAs<double>().value();
        if (explicitHeight > 0) contentHeight = explicitHeight;

        double explicitWidth = elementNode->getStyle().getProperty(StyleProperty::WIDTH).getAs<double>().value();
        if (explicitWidth > 0) {
            contentWidth = explicitWidth;
            // Проходим по детям и обрезаем их
            for (auto child : box->getChildren()) {
                // Складываем левый отступ + ширину блока
                if ((child->getWidth() + child->getX()) > contentWidth + x) {
                    double dif = child->getWidth() + child->getX() - (contentWidth + x);
                    child->setVisibleWidth(child->getWidth() - dif);
                }
            }
        }
        
        if (explicitWidth == 0 ||
            elementNode->getStyle().getProperty(StyleProperty::WIDTH).getLengthUnit() == StyleValue::LengthUnit::AUTO) {
            double maxChildWidth = 0;
            for (auto child : box->getChildren()) maxChildWidth = std::max(maxChildWidth, child->getWidth());
            
            if (maxChildWidth > 0) contentWidth = std::min(maxChildWidth, contentWidth);
        }

        if (explicitHeight == 0 || 
            elementNode->getStyle().getProperty(StyleProperty::HEIGHT).getLengthUnit() == StyleValue::LengthUnit::AUTO) {
            double maxChildHeight = 0;
            for (auto child : box->getChildren()) maxChildHeight += (child->getHeight() + child->getMarginY());
            
            if (maxChildHeight > 0) contentHeight = maxChildHeight;
        }
        
        // 6. Устанавливаем финальный размер
        double totalWidth = contentWidth + paddingLeft + paddingRight;
        double totalHeight = contentHeight + paddingTop + paddingBottom;

        box->setSize(totalWidth, totalHeight);

        box->setMarginY(marginBottom + marginTop + border*2);
        box->setMarginX(marginLeft + marginRight + border*2);

        availableHeight = totalHeight + marginTop + marginBottom + border*2;
    }
}

void LayoutEngine::computeInlineElement(LayoutBox* box, double& currentLineX, double& currentLineY, double& currentLineMaxHeight, double availableWidth) {
    // currentLineX - текущая позиция x в строке, currentLineY - текущая позиция y в строке, 
    // currentLineMaxHeight - максимальная высота строки, availableWidth - доступная ширина для размещения
    
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
             // Учитываем margin
            double marginTop = elementNode->getStyle().getProperty(StyleProperty::MARGIN_TOP).getAs<double>().value();
            double marginRight = elementNode->getStyle().getProperty(StyleProperty::MARGIN_RIGHT).getAs<double>().value();
            double marginBottom = elementNode->getStyle().getProperty(StyleProperty::MARGIN_BOTTOM).getAs<double>().value();
            double marginLeft = elementNode->getStyle().getProperty(StyleProperty::MARGIN_LEFT).getAs<double>().value();
        
            currentLineX += marginLeft;
            currentLineY += marginTop;

            // Учитываем padding
            double paddingLeft = elementNode->getStyle().getProperty(StyleProperty::PADDING_LEFT).getAs<double>().value();
            double paddingRight = elementNode->getStyle().getProperty(StyleProperty::PADDING_RIGHT).getAs<double>().value();
            double paddingTop = elementNode->getStyle().getProperty(StyleProperty::PADDING_TOP).getAs<double>().value();
            double paddingBottom = elementNode->getStyle().getProperty(StyleProperty::PADDING_BOTTOM).getAs<double>().value();

            // Дети начинают с ТЕКУЩЕЙ позиции в строке, а не с 0
            double childLineX = currentLineX + paddingLeft;
            double childLineY = currentLineY + paddingTop;
            double childLineMaxHeight = currentLineMaxHeight;
            
            for (auto child : box->getChildren()) 
                computeInlineElement(child, childLineX, childLineY, childLineMaxHeight, availableWidth);
            
            // Вычисляем общую ширину как разницу между конечной и начальной позицией
            contentWidth = childLineX - currentLineX;
            contentHeight = childLineMaxHeight;

            contentWidth += paddingLeft + paddingRight;
            contentHeight += paddingTop + paddingBottom + marginTop + marginBottom;
        }
        
        // Проверяем, помещается ли элемент в текущую строку
        if (currentLineX + contentWidth > availableWidth && currentLineX > 0) {
            // Перенос на новую строку
            currentLineX = 0;
            //currentLineY += currentLineMaxHeight;
            currentLineMaxHeight = 0;
        }
        
        // Устанавливаем позицию и размер
        box->setPosition(currentLineX, currentLineY);
        box->setSize(contentWidth, contentHeight);
        
        // Обновляем состояние строки для следующего элемента (так как работа идет через ссылку)
        currentLineX += contentWidth;
        currentLineMaxHeight = contentHeight;
        
        // Обработка inline-block элементов
        if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::INLINE_BLOCK) {
            // Inline-block ведет себя как block внутри, но как inline снаружи
            // После inline-block принудительный перенос строки
            currentLineX = 0;
            //currentLineY += currentLineMaxHeight;
            currentLineMaxHeight = 0;
        }
    }
}