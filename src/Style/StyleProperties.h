#ifndef STYLE_PROPERTIES_H
#define STYLE_PROPERTIES_H

enum class StyleProperty {
    
    // Обрабатываются на уровне координат и размеров блоков
    UNKNOWN, DISPLAY, WIDTH, HEIGHT, 
    MARGIN_TOP, MARGIN_RIGHT, MARGIN_BOTTOM, MARGIN_LEFT,
    PADDING_TOP, PADDING_RIGHT, PADDING_BOTTOM, PADDING_LEFT,

    // Обработка через текстовые метрики
    COLOR, FONT_SIZE, FONT_FAMILY, FONT_STYLE, FONT_WEIGHT,

    // Обработка со стороны графики
    BORDER_WIDTH, BORDER_COLOR,
    BACKGROUND_COLOR, BACKGROUND_IMAGE,
};

#endif