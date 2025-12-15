#include "TextMetrics.h"

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

StyleValue::FontDecorationType TextMetrics::getFontDecorationType() const {
    return fontDecorationType;
}

void TextMetrics::setFontDecorationType(StyleValue::FontDecorationType fontDecorationType_) {
    fontDecorationType = fontDecorationType_;
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

double TextMetrics::getCorrectedTextWidth(const std::string& text) const {
    // Проверяем задан ли Render который может по-другому вычислять метрику для шрифта
    // Если нет используем коэфициент 1, который определен в интерфейсе Renderer
    const Renderer* r = Renderer::get();
    if (!r) return getSingleLineWidth(text);

    return r->computeTextWidth(text, *this);
}

double TextMetrics::getCorrectedTextHeight(const std::string& text) const {
    const Renderer* r = Renderer::get();
    if (!r) return getCharHeight();

    return r->computeTextHeight(text, *this);
}

bool TextMetrics::isFontLoaded() const { return fontLoaded; }
std::string TextMetrics::getFontPath() const { return fontPath; }

double TextMetrics::getFontScale() const {
    if (fontSize <= 0) return 1.0;
    return fontSize / BASE_FONT_SIZE;
}

double TextMetrics::getStyleCoefficient() const {
    double coeff = 1.0;

    if (fontWeightType == StyleValue::FontWeightType::BOLD)
        coeff *= coefficients.bold;
    if (fontStyleType == StyleValue::FontStyleType::ITALIC)
        coeff *= coefficients.italic;

    return coeff;
}

double TextMetrics::getCharWidth() const {
    return BASE_CHAR_WIDTH * getStyleCoefficient() * getFontScale();
}

// итоговая высота строки
double TextMetrics::getCharHeight() const {
    return BASE_LINE_HEIGHT * getFontScale();
}

double TextMetrics::getSingleLineWidth(const std::string& text) const {
    if (text.empty()) return 0.0;
    return getCharWidth() * text.length();
}

double TextMetrics::getTextWidth(const std::string& text) const {
    if (text.empty()) return 0.0;

    double maxWidth = 0.0;

    size_t start = 0;
    size_t end = text.find('\n');

    while (true) {
        std::string line = text.substr(start, end - start);

        double corrected = getCorrectedTextWidth(line);
        if (corrected > maxWidth)
            maxWidth = corrected;

        if (end == std::string::npos)
            break;

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
    return lineCount * getCorrectedTextHeight("RANDOM text");
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
