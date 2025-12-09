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
            maxHeightContent = availableHeight;
        }

        if (childNode->getTagName() == "body") child->setSize(windowWidth, maxHeightContent);
    }

    rootBox->setSize(windowWidth, maxHeightContent); // Задаем ширину блока для html

    clippingElements(rootBox);
}

void LayoutEngine::clippingElements(LayoutBox* box) {
    // Размеры видимой области родителя
    double contentWidth  = box->getVisibleWidth();
    double contentHeight = box->getVisibleHeight();

    for (auto child : box->getChildren()) {

        ElementNode* node = dynamic_cast<ElementNode*>(child->getNode());
        double border = node != nullptr ? node->getStyle().getProperty(StyleProperty::BORDER_WIDTH).getAs<double>().value() : 0;

        // Обрезание по ширине 
        double oldVisibleWidth = child->getVisibleWidth();
        double oldVisibleHeight = child->getVisibleHeight();

        double childRight  = child->getX() + oldVisibleWidth;
        double parentRight = box->getX() + contentWidth;

        if (childRight > parentRight) {
            double overflow = childRight - parentRight;
            double newWidth = std::max(0.0, child->getVisibleWidth() - overflow);
            child->setVisibleWidth(newWidth);
        }

        // Работа с бордерами по X
        if (childRight + border > parentRight && child->getVisibleWidth() > 0) {
            double overflow = childRight + border - parentRight;
            child->setDynamicBorderX(overflow > border ? 0 : border - overflow);
        } else if (childRight - oldVisibleWidth >= parentRight) {
            double overflow = childRight - oldVisibleWidth - parentRight;
            child->setDynamicBorderX(overflow > border ? 0 : border - overflow);
        } else {
            if (!child->isOverflow()) {
                child->setDynamicBorderX(border);
                child->setOverflow(false);
            } else child->setDynamicBorderX(border);
        }

        // Обрезание по высоте
        double childBottom  = child->getY() + oldVisibleHeight;
        double parentBottom = box->getY() + contentHeight;

        if (childBottom > parentBottom) {
            double overflow = childBottom - parentBottom;
            double newHeight = std::max(0.0, child->getVisibleHeight() - overflow);
            child->setVisibleHeight(newHeight);
        }

        // Работа с бордерами по Y
        if (childBottom + border > parentBottom && child->getVisibleHeight() > 0) {
            double overflow = childBottom + border - parentBottom;
            child->setDynamicBorderY(overflow > border ? 0 : border - overflow);
        } else if (childBottom - oldVisibleHeight >= parentBottom) {
            double overflow = childBottom - oldVisibleHeight - parentBottom;
            child->setDynamicBorderY(overflow > border ? 0 : border - overflow);
        } else {
            if (!child->isOverflow()) {
                child->setDynamicBorderY(border);
                child->setOverflow(false);
            } else child->setDynamicBorderY(border);
        }
        
        clippingElements(child);
    }
}

void LayoutEngine::computeBlockElement(LayoutBox* box, double parentX, double parentY, double& availableWidth, double& availableHeight) {
    Node* node = box->getNode();
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node);

    if (elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::BLOCK) {
        
        // 1. Получаем отступы, margins, border
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
        double lineMaxHeight = 0;

        double offsetY = childY;

        bool flagInlineElement = false;
        double oldOffsetY = offsetY;
        LayoutBox* inlineBox = nullptr;

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

                if (flagInlineElement) {
                    // Корректируем родительский inline
                    inlineBox->setPosition(inlineBox->getX(), oldOffsetY);
                    inlineBox->setHeight(offsetY - oldOffsetY + lineMaxHeight);

                    offsetY += lineMaxHeight;
                    childY += lineMaxHeight;

                    flagInlineElement = false;
                }

                double childeHeight = availableHeight - childY;

                double parentWidth = elementNode->getStyle().getProperty(StyleProperty::WIDTH).getAs<double>().value();
                double localWindowWidth = windowWidth;
                computeBlockElement(child, childX, offsetY, parentWidth <= 0 ? localWindowWidth : parentWidth, childeHeight);

                offsetY += childeHeight;

                // Сбрасываем inline состояние
                lineX = childX;
                lineMaxHeight = 0;
            } else {
                // Inline ребенок (элемент или текстовый узел)
                flagInlineElement = true;

                double parentWidth = elementNode->getStyle().getProperty(StyleProperty::WIDTH).getAs<double>().value();
                double localWindowWidth = windowWidth;
                double maxWidth = 0;
                oldOffsetY = offsetY;
                
                computeInlineElement(child, lineX, offsetY, lineMaxHeight, parentWidth <= 0 ? localWindowWidth : parentWidth, maxWidth, childX);
                
                inlineBox = child;

                if (maxWidth != 0) child->setWidth(maxWidth);
            }
        }

        // 5. Вычисляем высоту блока
        double contentHeight = offsetY;

         // Проверяем явно заданную ширину и высоту и задаем относительно детей если ее нету
        double explicitHeight = elementNode->getStyle().getProperty(StyleProperty::HEIGHT).getAs<double>().value();
        if (explicitHeight > 0) contentHeight = explicitHeight;

        double explicitWidth = elementNode->getStyle().getProperty(StyleProperty::WIDTH).getAs<double>().value();

        if (explicitWidth > 0) contentWidth = explicitWidth;
        
        if (explicitWidth == 0 ||
            elementNode->getStyle().getProperty(StyleProperty::WIDTH).getLengthUnit() == StyleValue::LengthUnit::AUTO) {
            double maxChildWidth = 0;
            for (auto child : box->getChildren()) maxChildWidth = std::max(maxChildWidth, child->getWidth());
            
            if (maxChildWidth > 0) contentWidth = std::min(maxChildWidth, contentWidth);
        }

        if (explicitHeight == 0 || 
            elementNode->getStyle().getProperty(StyleProperty::HEIGHT).getLengthUnit() == StyleValue::LengthUnit::AUTO) {
            double maxChildHeight = 0;
            double maxY = 0;
    
            // вот тут нужно поменять логику и вычитать из последней координаты дочернего элеменат начальную координату родителя
            for (auto& child : box->getChildren()) {
                maxY = std::max(child->getY(), maxY);
                maxChildHeight = child->getHeight();
            }

            maxChildHeight = maxY - box->getY() + maxChildHeight;
            if (maxChildHeight > 0) contentHeight = maxChildHeight;
        }
        
        // 6. Устанавливаем финальный размер
        double totalWidth = contentWidth + paddingLeft + paddingRight;
        double totalHeight = contentHeight + paddingTop + paddingBottom;

        box->setSize(totalWidth, totalHeight);

        box->setMarginY(marginBottom + marginTop + border*2);
        box->setMarginX(marginLeft + marginRight + border*2);

        availableHeight = totalHeight + marginTop + marginBottom + border*2;
        availableWidth = contentWidth;
    }
}

void LayoutEngine::computeInlineElement(LayoutBox* box, double& currentLineX, double& currentLineY, double& currentLineMaxHeight, double availableWidth, double& maxWidth, double lineXstart) {
    // currentLineX - текущая позиция x в строке, currentLineY - текущая позиция y в строке, 
    // currentLineMaxHeight - максимальная высота строки, availableWidth - доступная ширина для размещения (от родителя)
    
    Node* node = box->getNode();
    ElementNode* elementNode = dynamic_cast<ElementNode*>(node);

    if ((elementNode != nullptr && elementNode->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::INLINE)
        || node->getType() == Node::Type::TEXT_NODE) {
        
        double contentWidth = 0;
        double contentHeight = 0;
        bool textBlock = false;
        
        if (node->getType() == Node::Type::TEXT_NODE) {
            // Текстовый узел - используем готовые метрики
            TextElement* textElement = dynamic_cast<TextElement*>(node);
            contentWidth = textElement->getTextMetrics().getTextWidth(textElement->getText());
            contentHeight = textElement->getTextMetrics().getTextHeight(textElement->getText());
            textBlock= true;
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
            currentLineY += paddingTop;
            double childLineMaxHeight = currentLineMaxHeight;
            
            std::cout << "!!!!!!!!!!currentLineX: " << currentLineX << std::endl;
            for (auto child : box->getChildren()) 
                computeInlineElement(child, childLineX, currentLineY, childLineMaxHeight, availableWidth, maxWidth, lineXstart);

            std::cout << "!!!!!!!!!!currentLineX: " << currentLineX << " " << childLineX << " " << lineXstart << " " << box->getNode()->getTagName() << " " << availableWidth << std::endl << std::endl;

            // Вычисляем общую ширину как разницу между конечной и начальной позицией
            if (childLineX < currentLineX) {
                contentWidth = currentLineX - childLineX;
                currentLineX -= contentWidth;
            } else contentWidth = childLineX - currentLineX;

            contentHeight = childLineMaxHeight;

            contentWidth += paddingLeft + paddingRight;
            contentHeight += paddingTop + paddingBottom + marginTop + marginBottom;
        }
        
        // Проверяем, помещается ли элемент в текущую строку
        if (textBlock && currentLineX + contentWidth - lineXstart > availableWidth && currentLineX > 0) {
            if (contentWidth > availableWidth) contentWidth = availableWidth, box->setOverflow(true);
            else currentLineX = lineXstart;

            // Перенос на новую строку
            currentLineY += currentLineMaxHeight;
            currentLineMaxHeight = contentHeight;

            std::cout << "currentLineY: " << currentLineY << std::endl;
        } else {
            maxWidth = std::max(maxWidth, contentWidth);
            currentLineMaxHeight = contentHeight; // тут надо будет сделать максимум в будущем
        }
    
        // Устанавливаем позицию и размер
        if (contentWidth == availableWidth) box->setPosition(lineXstart, currentLineY);
        else box->setPosition(currentLineX, currentLineY);

        box->setSize(contentWidth, contentHeight);
        
        // Обновляем состояние строки для следующего элемента (так как работа идет через ссылку)
        currentLineX += contentWidth;
    }
}