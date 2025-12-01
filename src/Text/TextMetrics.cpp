#include "TextMetrics.h"

// Значения как для обычного CSS (примерно)
static constexpr double BASE_FONT_SIZE = 16.0;     // px
static constexpr double BASE_CHAR_WIDTH = 7.0;     // px для 16px шрифта
static constexpr double BASE_LINE_HEIGHT = 14.0;   // px для 16px шрифта

void TextMetrics::setFontSize(double size) { fontSize = size; }
void TextMetrics::setColor(uint32_t color) { textColor = color; }

void TextMetrics::setFontStyleType(StyleValue::FontStyleType fontStyleType_) { 
    fontStyleType = fontStyleType_; 
}
void TextMetrics::setFontWeightType(StyleValue::FontWeightType fontWeightType_) { 
    fontWeightType = fontWeightType_; 
}

StyleValue::FontStyleType TextMetrics::getFontStyleType() const {
    return fontStyleType;
}

StyleValue::FontWeightType TextMetrics::getFontWeightType() const {
    return fontWeightType;
}

double TextMetrics::getFontSize() const { return fontSize; }
uint32_t TextMetrics::getTextColor() const { return textColor; }

bool TextMetrics::setFontFromFile(const std::string& path) {
    if (!checkFontFileExists(path)) {
        fontLoaded = false;
        return false;
    }
    
    if (!attemptToLoadFont(path)) {
        fontLoaded = false;
        return false;
    }
    
    fontPath = path;
    fontLoaded = true;
    return true;
}

bool TextMetrics::setFont(const std::string& fontName) {
    std::string fontPath = "/workspace/fonts/" + fontName + ".ttf";
    return setFontFromFile(fontPath);
}

bool TextMetrics::isFontLoaded() const { return fontLoaded; }
std::string TextMetrics::getFontPath() const { return fontPath; }

double TextMetrics::getFontScale() const {
    if (fontSize <= 0) return 1.0;
    return fontSize / BASE_FONT_SIZE;
}

double TextMetrics::getCharWidth(char c) const {
    double baseWidth;

    auto it = charWidthTable.find(c);
    if (it != charWidthTable.end()) {
        baseWidth = it->second;
    } else baseWidth = BASE_CHAR_WIDTH;
    

    return baseWidth * getFontScale() * getStyleCoefficient();
}

double TextMetrics::getStyleCoefficient() const {
    double coeff = 1.0;

    if (fontWeightType == StyleValue::FontWeightType::BOLD)
        coeff *= coefficients.bold;
    if (fontStyleType == StyleValue::FontStyleType::ITALIC)
        coeff *= coefficients.italic;

    return coeff;
}

// итоговая ширина символа
double TextMetrics::getEffectiveCharWidth() const {
    return BASE_CHAR_WIDTH * getFontScale() * getStyleCoefficient();
}

// итоговая высота строки
double TextMetrics::getEffectiveLineHeight() const {
    return BASE_LINE_HEIGHT * getFontScale();
}

double TextMetrics::getSingleLineWidth(const std::string& text) const {
    if (text.empty()) return 0.0;

    double width = 0.0;

    for (char c : text)
        width += getCharWidth(c);

    auto it = charWidthTable.find(' ');
    double spaceWidth = (it != charWidthTable.end() ? it->second : BASE_CHAR_WIDTH);
    width += spaceWidth * getFontScale() * getStyleCoefficient() * 1.4;

    return width;
}

double TextMetrics::getTextWidth(const std::string& text) const {
    if (text.empty()) return 0.0;

    double maxWidth = 0.0;
    size_t start = 0;
    size_t end = text.find('\n');
    
    while (true) {
        std::string line = text.substr(start, end - start);
        double lineWidth = getSingleLineWidth(line);
        if (lineWidth > maxWidth) maxWidth = lineWidth;
        
        if (end == std::string::npos) break;
        start = end + 1;
        end = text.find('\n', start);
    }
    
    return maxWidth;
}

double TextMetrics::getTextHeight(const std::string& text) const {
    if (text.empty()) return 0.0;

    int lineCount = 1;
    for (char c : text) {
        if (c == '\n') lineCount++;
    }

    return getTextHeightForLines(lineCount);
}

double TextMetrics::getTextHeightForLines(int lineCount) const {
    return lineCount * getEffectiveLineHeight();
}

bool TextMetrics::checkFontFileExists(const std::string& path) const {
    std::ifstream file(path);
    return file.good();
}

bool TextMetrics::attemptToLoadFont(const std::string& path) {
    std::string extension = path.substr(path.find_last_of(".") + 1);
    
    std::vector<std::string> supportedExtensions = { "ttf" };
    
    for (const auto& ext : supportedExtensions) {
        if (extension == ext) return true;
    }
    return false;
}
