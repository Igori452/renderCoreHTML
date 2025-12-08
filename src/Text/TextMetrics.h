#ifndef TEXT_METRICS_H
#define TEXT_METRICS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "../Style/StyleValue.h"

class TextMetrics {
private:    
    // Коэффициенты для разных стилей текста
    struct StyleCoefficients {
        double bold = 1.07;              // жирный текст шире на 20%
        double italic = 1.02;            // курсив немного шире
    } coefficients;
    
    double fontSize;
    std::string fontPath;
    bool fontLoaded;

    uint32_t textColor;
    StyleValue::FontStyleType fontStyleType;
    StyleValue::FontWeightType fontWeightType;
    StyleValue::FontDecorationType fontDecorationType;

    std::unordered_map<char, double> charWidthTable = {
        // Узкие
        {'i', 5.2}, {'l', 5.2}, {'j', 5.0}, {'t', 6.2},

        // Средние
        {'a', 6.8}, {'c', 6.7}, {'e', 7.0}, {'o', 7.1}, {'n', 7.1},
        {'r', 6.2}, {'s', 6.6}, {'u', 7.0}, {'v', 7.1}, {'d', 7.3},

        // Широкие
        {'m', 9.6}, {'w', 10.6},

        // Прописные
        {'A', 8.4}, {'B', 8.6}, {'C', 8.7}, {'D', 9.2}, {'E', 8.3},
        {'H', 9.1}, {'M', 11.3}, {'W', 12.5},

        // Пробел и пунктуация
        {' ', 5.4}, {'.', 4.2}, {',', 4.2}, {'!', 5.1}, {'?', 7.2}
    };

    double getCharWidth(char c) const;

    double getStyleCoefficient() const;
    bool checkFontFileExists(const std::string& path) const;
    bool attemptToLoadFont(const std::string& path);

    double getFontScale() const;
    double getEffectiveCharWidth() const;
    double getEffectiveLineHeight() const;

    bool setFontFromFile(const std::string& path);
public:
    TextMetrics() = default;
        
    void setFontSize(double size);
    void setColor(uint32_t color);
    
    void setFontStyleType(StyleValue::FontStyleType fontStyleType_);
    void setFontWeightType(StyleValue::FontWeightType fontWeightType_);
    void setFontDecorationType(StyleValue::FontDecorationType fontDecorationType_);

    StyleValue::FontStyleType getFontStyleType() const;
    StyleValue::FontWeightType getFontWeightType() const;
    StyleValue::FontDecorationType getFontDecorationType() const;
    double getFontSize() const;
    uint32_t getTextColor() const;

    bool setFont(const std::string& fontName);

    bool isFontLoaded() const;
    std::string getFontPath() const;

    double getTextWidth(const std::string& text) const;
    double getTextHeight(const std::string& text) const;
    double getTextHeightForLines(int lineCount) const;
    
    double getSingleLineWidth(const std::string& text) const;
};

#endif