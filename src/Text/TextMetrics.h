#ifndef TEXT_METRICS_H
#define TEXT_METRICS_H

#include <unordered_map>
#include <vector>
#include <fstream>
#include "../Style/StyleValue.h"
#include "../Render/Renderer.h"

class TextMetrics {
private:    
    static constexpr double BASE_FONT_SIZE = 16.0;     // px
    static constexpr double BASE_CHAR_WIDTH = 7.0;     // px
    static constexpr double BASE_LINE_HEIGHT = 20.0;   // px 

    // Коэффициенты для разных стилей текста
    struct StyleCoefficients {
        double bold = 1.07;    // жирный текст шире 
        double italic = 1.02;  // курсив немного шире
    } coefficients;
    
    double fontSize;
    std::string fontPath;
    bool fontLoaded;

    uint32_t textColor;
    StyleValue::FontStyleType fontStyleType;
    StyleValue::FontWeightType fontWeightType;
    StyleValue::FontDecorationType fontDecorationType;

    // Универсальные метрики символа
    double getStyleCoefficient() const;
    double getFontScale() const;
    double getCharWidth() const;
    double getCharHeight() const;

    bool checkFontFileExists(const std::string& path) const;
    bool attemptToLoadFont(const std::string& path);

    bool setFontFromFile(const std::string& path);

    // Реализация паттерна "Стратегия"
    double getCorrectedTextWidth(const std::string& text) const;
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