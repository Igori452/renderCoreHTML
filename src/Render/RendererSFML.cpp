#include "RendererSFML.h"

RendererSFML::RendererSFML() : window(sf::VideoMode(800, 600), "SFML - Mini Browser") {}

void RendererSFML::drawElement(LayoutBox& layoutBox) {

    Node* node = layoutBox.getNode();

    double x = layoutBox.getX();
    double y = layoutBox.getY();
    double width = layoutBox.getWidth();
    double height = layoutBox.getHeight();

    double visibleWidth = layoutBox.getVisibleWidth();
    double visibleHeight = layoutBox.getVisibleHeight();

    float renderY = y - scrollOffset;

    // Пропуск элементов вне экрана
    if (renderY + height < 0 || renderY > windowHeight) return;

    bool hasBackgroundImage = false;

    int borderWidth = 0;

    int borderRight = layoutBox.getDynamicBorderX();
    int borderBottom = layoutBox.getDynamicBorderY();

    sf::Color borderColor = sf::Color::Transparent;

    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;

    int clippedX = 0;
    int clippedY = 0;

    if (node->getType() == Node::Type::ELEMENT_NODE) {

        ElementNode* elementNode = dynamic_cast<ElementNode*>(node);
        auto styleMap = elementNode->getStyle().getMapPropertyMerge();

        //  Если overflow: hidden создаём clip-буфер
        sf::RenderTexture* clipBuffer = nullptr;

        if (layoutBox.isOverflow()) {
            borderWidth = styleMap.find(StyleProperty::BORDER_WIDTH)->second.getAs<double>().value();

            clippedX = borderWidth;
            clippedY = borderWidth;

            int widthBufferSize = 0;
            int heightBufferSize = 0;
            
            if (visibleWidth > 0) widthBufferSize = visibleWidth + borderWidth + borderRight;
            else if (borderRight > 0) widthBufferSize = borderRight;

            if (visibleHeight > 0) heightBufferSize = visibleHeight + borderWidth + borderBottom;
            else if (borderBottom > 0) heightBufferSize = borderBottom;

            clipBuffer = new sf::RenderTexture();
            if (visibleWidth > 0 && visibleHeight > 0) {
                clipBuffer->create(
                    (unsigned)(widthBufferSize),
                    (unsigned)(heightBufferSize)
                );
                clipBuffer->clear(sf::Color::Transparent);
            }
        }

        // Выбираем, куда рисовать: окно или буфер
        sf::RenderTarget& target = clipBuffer ? (sf::RenderTarget&)*clipBuffer : (sf::RenderTarget&)window;

        // Рисуем сам прямоугольник элемента
        sf::RectangleShape elementRect(sf::Vector2f(width, height));

        if (!clipBuffer)
            elementRect.setPosition(x, renderY);
        else
            elementRect.setPosition(clippedX, clippedY);

        // Обрабатываем стили
        for (auto& item : styleMap) {
            switch (item.first) {

                case StyleProperty::BACKGROUND_COLOR: {
                    auto colorValue = item.second.getAs<uint32_t>().value();

                    elementRect.setFillColor(sf::Color(
                        (colorValue >> 24) & 0xFF,
                        (colorValue >> 16) & 0xFF,
                        (colorValue >> 8) & 0xFF,
                        colorValue & 0xFF
                    ));
                    break;
                }

                case StyleProperty::BORDER_COLOR: {
                    auto v = item.second.getAs<uint32_t>().value();
                    borderColor = sf::Color(
                        (v >> 24) & 0xFF,
                        (v >> 16) & 0xFF,
                        (v >> 8)  & 0xFF,
                        v        & 0xFF
                    );
                    elementRect.setOutlineColor(borderColor);
                    break;
                }

                // Рисуем border для overflow элементов по осбенному
                case StyleProperty::BORDER_WIDTH: {
                    if (!layoutBox.isOverflow()) 
                        elementRect.setOutlineThickness((float)item.second.getAs<double>().value());
                    break;
                }

                case StyleProperty::BACKGROUND_IMAGE: {
                    std::string fileName = item.second.getAs<std::string>().value();
                    if (fileName.empty()) break;

                    std::string imagePath = "/workspace/images/" + fileName;

                    if (backgroundTexture.loadFromFile(imagePath)) {
                        backgroundSprite.setTexture(backgroundTexture);
                        hasBackgroundImage = true;

                        if (!clipBuffer)
                            backgroundSprite.setPosition(x, renderY);
                        else
                            backgroundSprite.setPosition(clippedX, clippedY);

                        auto size = backgroundTexture.getSize();
                        backgroundSprite.setScale(
                            width  / size.x,
                            height / size.y
                        );
                    }
                    break;
                }

                default: break;
            }
        }

        // Рисуем фон
        target.draw(elementRect);
        if (hasBackgroundImage)
            target.draw(backgroundSprite);

        // Если был clip переносим в окно
        if (clipBuffer) {

            if (borderWidth > 0) {
                // 1. TOP border
                sf::RectangleShape top(sf::Vector2f(width + borderWidth, borderWidth));
                top.setFillColor(borderColor);
                top.setPosition(0, 0);
                clipBuffer->draw(top);

                // 2. BOTTOM border
                sf::RectangleShape bottom(sf::Vector2f(width + borderWidth*2, borderWidth));
                bottom.setFillColor(borderColor);
                bottom.setPosition(0, height + borderWidth);
                clipBuffer->draw(bottom);

                // 3. LEFT border
                sf::RectangleShape left(sf::Vector2f(borderWidth, height + borderWidth));
                left.setFillColor(borderColor);
                left.setPosition(0, 0);
                clipBuffer->draw(left);

                // 4. RIGHT border
                sf::RectangleShape right(sf::Vector2f(borderWidth, height + borderWidth));
                right.setFillColor(borderColor);
                right.setPosition(width + borderWidth, 0);
                clipBuffer->draw(right);
            }

            clipBuffer->display();

            sf::Sprite clipped(clipBuffer->getTexture());
            clipped.setPosition(x - clippedX, renderY - clippedY);
            window.draw(clipped);

            delete clipBuffer;
        }
    }
}

void RendererSFML::drawText(LayoutBox& layoutBox) {
    Node* node = layoutBox.getNode();

    double x = layoutBox.getX();
    double y = layoutBox.getY();
    double width = layoutBox.getWidth();
    double height = layoutBox.getHeight();

    double visibleWidth = layoutBox.getVisibleWidth();
    double visibleHeight = layoutBox.getVisibleHeight();

    if (visibleWidth == 0 || visibleHeight == 0) return;

    float renderY = y - scrollOffset;

    if (renderY + height < 0 || renderY > windowHeight)
        return;

    if (node->getType() != Node::Type::TEXT_NODE)
        return;

    TextElement* textElement = dynamic_cast<TextElement*>(node);
    TextMetrics textMetrics = textElement->getTextMetrics();

    std::string textContent = textElement->getText();
    if (textContent.empty()) return;

    double fontSize = textMetrics.getFontSize();
    uint32_t textColorValue = textMetrics.getTextColor();
    std::string fontPath = textMetrics.getFontPath();

    // -----------------------
    // 1. Создаём объект текста
    // -----------------------
    sf::Font* font = new sf::Font();
    font->loadFromFile(fontPath);

    sf::Text text;
    text.setString(textContent);
    text.setCharacterSize(static_cast<unsigned int>(fontSize));
    text.setFont(*font);

    // Цвет
    uint8_t r = (textColorValue >> 24) & 0xFF;
    uint8_t g = (textColorValue >> 16) & 0xFF;
    uint8_t b = (textColorValue >> 8)  & 0xFF;
    uint8_t a =  textColorValue        & 0xFF;
    text.setFillColor(sf::Color(r,g,b,a));

    // Стиль
    sf::Uint32 style = sf::Text::Regular;
    if (textMetrics.getFontWeightType() == StyleValue::FontWeightType::BOLD) style |= sf::Text::Bold;
    if (textMetrics.getFontStyleType()  == StyleValue::FontStyleType::ITALIC) style |= sf::Text::Italic;
    if (textMetrics.getFontDecorationType()  == StyleValue::FontDecorationType::UNDERLINED) style |= sf::Text::Underlined;
    else if (textMetrics.getFontDecorationType()  == StyleValue::FontDecorationType::LINE_THROUGH) style |= sf::Text::StrikeThrough;

    text.setStyle(style);

    // Корректировка вертикального оффсета
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(0, bounds.top);

    // 2. Проверяем overflow
    bool clip = layoutBox.isOverflow();

    if (!clip) {
        // Без обрезания — обычный вывод
        text.setPosition(x, renderY);
        window.draw(text);
        delete font;
        return;
    }

    // 3. Рендерим текст в clip-buffer
    unsigned clipW = (unsigned)layoutBox.getVisibleWidth();
    unsigned clipH = (unsigned)layoutBox.getVisibleHeight();

    sf::RenderTexture clipBuffer;
    clipBuffer.create(clipW, clipH);
    clipBuffer.clear(sf::Color::Transparent);

    // Позиция внутри буфера всегда (0,0)
    text.setPosition(0, 0);

    clipBuffer.draw(text);
    clipBuffer.display();

    // -----------------------
    // 4. Переносим в окно
    // -----------------------
    sf::Sprite clipped(clipBuffer.getTexture());
    clipped.setPosition(x, renderY);

    window.draw(clipped);

    delete font;
}


// Рекурсивная функция обхода дерева LayoutBox
void RendererSFML::renderLayoutTree(LayoutBox& layoutBox) {
    Node* node = layoutBox.getNode();

    // Сначала рисуем ВСЕ блоки
    if (node->getType() == Node::Type::ELEMENT_NODE && 
        dynamic_cast<ElementNode*>(node)->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::BLOCK) {
        drawElement(layoutBox);
    }
    
    // Затем обходим детей
    auto children = layoutBox.getChildren();
    for (auto& child : children) {
        renderLayoutTree(*child);
    }
    
    // И только потом рисуем текст (ПОВЕРХ блоков)
    if (node->getType() == Node::Type::TEXT_NODE || (node->getType() == Node::Type::ELEMENT_NODE &&
        dynamic_cast<ElementNode*>(node)->getStyle().getProperty(StyleProperty::DISPLAY).getAs<StyleValue::DisplayType>().value() == StyleValue::DisplayType::INLINE)) {
        drawText(layoutBox);
    }
}

void RendererSFML::showScene(LayoutBox& rootLayoutBox) {
    contentHeight = rootLayoutBox.getHeight();

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            float maxScroll = std::max(0.f, contentHeight - windowHeight);

            scrollOffset -= event.mouseWheelScroll.delta * 40.f;

            if (scrollOffset < 0) scrollOffset = 0;
            if (scrollOffset > maxScroll) scrollOffset = maxScroll;

        }

        window.clear(sf::Color::White);
        renderLayoutTree(rootLayoutBox);
        window.display();
    }
}
