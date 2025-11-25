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
        double bold = 1.2;              // жирный текст шире на 20%
        double italic = 1.1;            // курсив немного шире
    } coefficients;
    
    double fontSize;
    std::string fontPath;
    bool fontLoaded;

    uint32_t textColor;
    StyleValue::FontStyleType fontStyleType;
    StyleValue::FontWeightType fontWeightType;

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

    StyleValue::FontStyleType getFontStyleType() const;
    StyleValue::FontWeightType getFontWeightType() const;
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