#ifndef TEXT_METRICS_H
#define TEXT_METRICS_H

#include <string>
#include <unordered_map>
#include <vector>

class TextMetrics {
private:
    // Базовые константы - можно легко поменять под свои нужды
    double charWidth = 8.0;             // ширина одного символа в пикселях
    double lineHeight = 18.0;           // высота строки в пикселях
    double fontSize = 16.0;             // базовый размер шрифта (для совместимости)
    uint32_t textColor = 0x000000FF;    // цвет текста (по умолчанию черный)

public:
    TextMetrics() = default;
    
    // Можно настроить базовые размеры если нужно
    void setCharWidth(double width);
    void setLineHeight(double height);
    void setFontSize(double size);
    void setColor(uint32_t color);
    
    double getTextWidth(const std::string& text) const;
    double getTextHeight(const std::string& text) const;
    double getTextHeightForLines(int lineCount);
    double getFontSize() const;
    uint32_t getTextColor() const;
};

#endif