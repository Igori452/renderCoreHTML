#include "RendererSFML.h"

RendererSFML::RendererSFML() : window(sf::VideoMode(800, 600), "SFML - Mini Browser") {}

void RendererSFML::drawElement(LayoutBox& layoutBox) {
    Node* node = layoutBox.getNode();

    double x = layoutBox.getX();
    double y = layoutBox.getY();
    double width = layoutBox.getWidth();
    double height = layoutBox.getHeight();

    // Отрисовка блочных элементов
    if (node->getType() == Node::Type::ELEMENT_NODE) {
        ElementNode* elementNode = dynamic_cast<ElementNode*>(node);
        auto styleMap = elementNode->getStyle().getMapProperty();

        // Создаем прямоугольник для элемента
        sf::RectangleShape elementRect(sf::Vector2f(width, height));
        elementRect.setPosition(x, y);

        // Обрабатываем стили
        for (auto& item : styleMap) {
            switch (item.first) {
                case StyleProperty::BACKGROUND_COLOR: {
                    auto colorValue = item.second.getAs<uint32_t>().value();

                    // Конвертируем uint32_t в sf::Color (RGBA) и сразу применяем
                    /*
                    elementRect.setFillColor(sf::Color(
                        (colorValue >> 24) & 0xFF,  // R
                        (colorValue >> 16) & 0xFF,  // G
                        (colorValue >> 8) & 0xFF,   // B
                        colorValue & 0xFF           // A
                    ));
                    */

                    elementRect.setFillColor(sf::Color::Transparent);
                    break;
                }
                case StyleProperty::BORDER_COLOR: {
                    auto colorValue = item.second.getAs<uint32_t>().value();

                    // Конвертируем и сразу применяем цвет границы
                    elementRect.setOutlineColor(sf::Color(
                        (colorValue >> 24) & 0xFF,
                        (colorValue >> 16) & 0xFF,
                        (colorValue >> 8) & 0xFF,
                        colorValue & 0xFF
                    ));

                    break;
                }
                case StyleProperty::BORDER_WIDTH: {
                    auto borderWidthValue = item.second.getAs<double>().value();
                    // Сразу применяем толщину границы
                    elementRect.setOutlineThickness(static_cast<float>(borderWidthValue));

                    break;
                }

                default:
                    break;
            }
        }
        
        window.draw(elementRect);
    }
}


void RendererSFML::drawText(LayoutBox& layoutBox) {
    Node* node = layoutBox.getNode();

    double x = layoutBox.getX();
    double y = layoutBox.getY();
    double width = layoutBox.getWidth();
    double height = layoutBox.getHeight();

    if (node->getType() == Node::Type::TEXT_NODE) {
        TextElement* textElement = dynamic_cast<TextElement*>(node);
        TextMetrics textMetrics = textElement->getTextMetrics();

        double fontSize = textMetrics.getFontSize();
        uint32_t textColorValue = textMetrics.getTextColor();
        std::string textContent = textElement->getText();

        if (!textContent.empty()) {
            // Создаем sfml текст 
            sf::Text text;
            text.setString(textContent);
            text.setCharacterSize(static_cast<unsigned int>(fontSize));
            
            // Конвертируем цвет
            sf::Color textColor(
                (textColorValue >> 24) & 0xFF,  // R
                (textColorValue >> 16) & 0xFF,  // G
                (textColorValue >> 8) & 0xFF,   // B
                textColorValue & 0xFF           // A
            );
            text.setFillColor(textColor);
            
            // Позиционируем текст
            text.setPosition(x, y);
            
            // Отрисовываем
            window.draw(text);
        }
    }
}

// Рекурсивная функция обхода дерева LayoutBox
void RendererSFML::renderLayoutTree(LayoutBox& layoutBox) {
    Node* node = layoutBox.getNode();
    
    // Сначала рисуем ВСЕ блоки
    
    if (node->getType() == Node::Type::ELEMENT_NODE) {
        drawElement(layoutBox);
    }
    
    // Затем обходим детей
    auto children = layoutBox.getChildren();
    for (auto& child : children) {
        renderLayoutTree(*child);
    }
    
    // И только потом рисуем текст (ПОВЕРХ блоков)
    if (node->getType() == Node::Type::TEXT_NODE) {
        drawText(layoutBox);
    }
}

/*
void RendererSFML::showScene(LayoutBox& rootLayoutBox) {
    while (window.isOpen()) {
        window.clear(sf::Color::White);

        renderLayoutTree(rootLayoutBox);

        window.display();
    }
}*/

void RendererSFML::showScene(LayoutBox& rootLayoutBox) {
    // Рисуем простейшие примитивы
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(100, 100);
    
    sf::Text testText;
    testText.setString("SFML WORKS!");
    testText.setCharacterSize(30);
    testText.setFillColor(sf::Color::Red);
    testText.setPosition(200, 200);

    while (window.isOpen()) {
        window.clear(sf::Color::White);
        
        // Рисуем тестовые объекты
        window.draw(circle);
        window.draw(testText);
        
        // НЕ рисуем наш layout вообще
        // renderLayoutTree(rootLayoutBox);
        
        window.display();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
    }
}