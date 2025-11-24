#include "TextMetrics.h"

void TextMetrics::setAvgCharWidth(double width) { avgCharWidth = width; }
void TextMetrics::setLineHeight(double height) { lineHeight = height; }
void TextMetrics::setFontSize(double size) { fontSize = size; }
void TextMetrics::setColor(uint32_t color) { textColor = color; }


void TextMetrics::setBoldCoefficient(double coeff) { coefficients.bold = coeff; }
void TextMetrics::setItalicCoefficient(double coeff) { coefficients.italic = coeff; }
void TextMetrics::setCondensedCoefficient(double coeff) { coefficients.condensed = coeff; }
void TextMetrics::setMonospaceCoefficient(double coeff) { coefficients.monospace = coeff; }


void TextMetrics::setBold(bool bold_) { bold = bold_; }
void TextMetrics::setItalic(bool italic_) { italic = italic_; }
void TextMetrics::setCondensed(bool condensed_) { condensed = condensed_; }
void TextMetrics::setMonospace(bool monospace_) { monospace = monospace_; }

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

bool TextMetrics::isFontLoaded() const { return fontLoaded; }
std::string TextMetrics::getFontPath() const { return fontPath; }
std::string TextMetrics::getFontFamily() const { return fontFamily; }

double TextMetrics::getTextWidth(const std::string& text) const {
    if (text.empty()) return 0.0;
    
    // Для многострочного текста берем самую длинную строку
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
    
    // Считаем количество строк
    int lineCount = 1;
    for (char c : text) {
        if (c == '\n') lineCount++;
    }
    
    return getTextHeightForLines(lineCount);
}

double TextMetrics::getAdjustedCharWidth() const {
    return avgCharWidth * getStyleCoefficient();
}

double TextMetrics::getSingleLineWidth(const std::string& text) const {
    if (text.empty()) return 0.0;
    
    double adjustedWidth = getAdjustedCharWidth();
    return text.length() * adjustedWidth;
}

double TextMetrics::getTextHeightForLines(int lineCount) const {
    return lineCount * lineHeight;
}

int TextMetrics::estimateCharCountForWidth(double width) const {
    double adjustedWidth = getAdjustedCharWidth();
    return static_cast<int>(width / adjustedWidth);
}

double TextMetrics::getStyleCoefficient() const {
    double coeff = 1.0;
    
    if (bold) coeff *= coefficients.bold;
    if (italic) coeff *= coefficients.italic;
    if (condensed) coeff *= coefficients.condensed;
    if (monospace) coeff *= coefficients.monospace;
    
    return coeff;
}

bool TextMetrics::checkFontFileExists(const std::string& path) const {
    // Простая проверка существования файла
    std::ifstream file(path);
    return file.good();
}

bool TextMetrics::attemptToLoadFont(const std::string& path) {
    std::string extension = path.substr(path.find_last_of(".") + 1);
    
    // Список поддерживаемых расширений шрифтов
    std::vector<std::string> supportedExtensions = {
        "ttf"
    };
    
    for (const auto& ext : supportedExtensions) {
        if (extension == ext) {
            return true; // Расширение поддерживается
        }
    }
    
    return false;
}

bool TextMetrics::isBold() const { return bold; }
bool TextMetrics::isItalic() const { return italic; }
bool TextMetrics::isCondensed() const { return condensed; }
bool TextMetrics::isMonospace() const { return monospace; }