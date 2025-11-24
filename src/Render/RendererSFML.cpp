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
                    
                    elementRect.setFillColor(sf::Color(
                        (colorValue >> 24) & 0xFF,  // R
                        (colorValue >> 16) & 0xFF,  // G
                        (colorValue >> 8) & 0xFF,   // B
                        colorValue & 0xFF           // A
                    ));
                    
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
        std::string fontPath = textMetrics.getFontPath();

        if (!textContent.empty()) {
            // Создаем sfml текст 
            sf::Text text;

            text.setString(textContent);
            text.setCharacterSize(static_cast<unsigned int>(fontSize));
            
            // Загружаем шрифт (попробуем с кэшированием)
            static sf::Font font;
            static bool fontLoaded = false;

            if (!fontLoaded) {

                std::vector<std::string> fontPaths = {
                    "/usr/local/share/fonts/arial.ttf",
                    "/usr/local/share/fonts/arialmt.ttf",
                    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
                };

                for (const auto& path : fontPaths) {
                    if (font.loadFromFile(path)) {
                        std::cout << "Font loaded: " << path << std::endl;
                        fontLoaded = true;
                        break;
                    } else {
                        std::cout << "Failed to load: " << path << std::endl;
                    }
                }

                if (!fontLoaded) {
                    std::cout << "ERROR: cannot load ANY font!" << std::endl;
                }
            }

            text.setFont(font);
            
            // Применяем стили текста
            if (textMetrics.isBold()) {
                text.setStyle(sf::Text::Bold);
            }
            if (textMetrics.isItalic()) {
                text.setStyle(sf::Text::Italic);
            }
            
            // Конвертируем цвет
            uint8_t a = (textColorValue >> 24) & 0xFF;
            uint8_t r = (textColorValue >> 16) & 0xFF;
            uint8_t g = (textColorValue >> 8) & 0xFF;
            uint8_t b = textColorValue & 0xFF;

            sf::Color textColor(r, g, b, a);
            
            // Позиционируем текст с учетом границ
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(0, bounds.top);  // вертикальное выравнивание
            text.setPosition(x, y);

            text.setFillColor(sf::Color(r, g, b, 255));

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


void RendererSFML::showScene(LayoutBox& rootLayoutBox) {
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        renderLayoutTree(rootLayoutBox);
        window.display();
    }
}
