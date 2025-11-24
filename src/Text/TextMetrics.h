#ifndef TEXT_METRICS_H
#define TEXT_METRICS_H

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

class TextMetrics {
private:
    // Базовые константы
    double avgCharWidth = 8.0;          // средняя ширина символа в пикселях
    double lineHeight = 18.0;           // высота строки в пикселях
    double fontSize = 16.0;             // базовый размер шрифта
    
    // Коэффициенты для разных стилей текста
    struct StyleCoefficients {
        double bold = 1.2;              // жирный текст шире на 20%
        double italic = 1.1;            // курсив немного шире
        double condensed = 0.8;         // узкий шрифт
        double monospace = 1.15;        // моноширинный шрифт
    } coefficients;
    
    // Текущие настройки стиля
    bool bold = false;
    bool italic = false;
    bool condensed = false;
    bool monospace = false;
    
    std::string fontPath = "C:\\work\\docker\\miniBrowser\\fonts\\arialmt.ttf";
    bool fontLoaded = false;
    std::string fontFamily = "Arial";   // семейство шрифтов по умолчанию

    uint32_t textColor = 0x00000000;    // цвет текста

    double getStyleCoefficient() const;
    bool checkFontFileExists(const std::string& path) const;
    bool attemptToLoadFont(const std::string& path);

public:
    TextMetrics() = default;
    
    // Настройка базовых размеров
    void setAvgCharWidth(double width);
    void setLineHeight(double height);
    void setFontSize(double size);
    void setColor(uint32_t color);
    
    // Настройка коэффициентов стилей
    void setBoldCoefficient(double coeff);
    void setItalicCoefficient(double coeff);
    void setCondensedCoefficient(double coeff);
    void setMonospaceCoefficient(double coeff);
    
    // Управление стилями
    void setBold(bool bold_);
    void setItalic(bool italic_);
    void setCondensed(bool condensed_);
    void setMonospace(bool monospace_);

    // Работа со шрифтами
    bool setFontFromFile(const std::string& path);
    bool isFontLoaded() const;
    std::string getFontPath() const;
    std::string getFontFamily() const;
    
    // Основные методы расчета
    double getTextWidth(const std::string& text) const;
    double getTextHeight(const std::string& text) const;
    double getTextHeightForLines(int lineCount) const;
    double getFontSize() const;
    uint32_t getTextColor() const;
    
    // Дополнительные полезные методы
    double getSingleLineWidth(const std::string& text) const;
    double getAdjustedCharWidth() const;
    int estimateCharCountForWidth(double width) const;

    bool isBold() const;
    bool isItalic() const;
    bool isCondensed() const;
    bool isMonospace() const;
};

#endif