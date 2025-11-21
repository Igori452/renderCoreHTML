#include "TextMetrics.h"

// Можно настроить базовые размеры если нужно
void TextMetrics::setCharWidth(double width) { charWidth = width; }
void TextMetrics::setLineHeight(double height) { lineHeight = height; }
void TextMetrics::setFontSize(double size) { fontSize = size; }
void TextMetrics::setColor(uint32_t color) { textColor = color; }

// Основной метод - вычисляет ширину текста
double TextMetrics::getTextWidth(const std::string& text) const {
    return text.length() * charWidth;
}

// Вычисляет высоту текста (для одной строки)
double TextMetrics::getTextHeight(const std::string& text) const {
    return lineHeight;
}

// Вычисляет высоту для нескольких строк
double TextMetrics::getTextHeightForLines(int lineCount) {
    return lineCount * lineHeight;
}

double TextMetrics::getFontSize() const {
    return fontSize;
}

uint32_t TextMetrics::getTextColor() const {
    return textColor;
}