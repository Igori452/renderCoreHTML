#ifndef STYLE_VALUE_H
#define STYLE_VALUE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <optional>
#include "StyleProperties.h"

class StyleValue {
    public:
        enum class LengthUnit { UNDEFINED, PX };

        enum class Type { UNDEFINED, DISPLAY, DOUBLE, COLOR, FONT, FONT_STYLE, FONT_WEIGHT };

        enum class DisplayType { INLINE, BLOCK, INLINE_BLOCK };

        enum class FontStyleType { NORMAL, ITALIC, UNDERLINED };
        enum class FontWeightType { NORMAL, BOLD };

    private:
        Type type {Type::UNDEFINED};

        LengthUnit lengthUnit;

        DisplayType displayType;
        FontStyleType fontStyleType;
        FontWeightType fontWeightType;

        double numericValue;
        uint32_t colorValue;
        std::string font;

        static bool parseColor(const std::string& stringColor,  uint32_t& color);
        
        static StyleValue setStyle();
        static StyleValue setStyle(DisplayType _value);
        static StyleValue setStyle(FontStyleType _value);
        static StyleValue setStyle(FontWeightType _value);
        static StyleValue setStyle(std::string _value);
        static StyleValue setStyle(double _value, LengthUnit lengthUnit_);
        static StyleValue setStyle(uint32_t _value);

        static LengthUnit getLengthUnitFromString(const std::string& LengthUnitString);

        DisplayType getDisplayType() const;
        double getNumericValue() const;
        uint32_t getColorValue() const;
        LengthUnit getLengthUnit() const;

    public:
        static StyleValue setStyleValueFromString(StyleProperty property, const std::string& stringValue, const std::string& lengthUnitString_ = "");

        Type getType() const;

        // Универсальный геттер
        template<typename T>
        std::optional<T> getAs() const {
            if constexpr (std::is_same_v<T, DisplayType>) {
                if (type == Type::DISPLAY) return displayType;
            } else if constexpr (std::is_same_v<T, double>) {
                if (type == Type::DOUBLE) return numericValue;
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                if (type == Type::COLOR) return colorValue;
            } else if constexpr (std::is_same_v<T, FontStyleType>) {
                if (type == Type::FONT_STYLE) return fontStyleType;
            } else if constexpr (std::is_same_v<T, FontWeightType>) {
                if (type == Type::FONT_WEIGHT) return fontWeightType;
            } else if constexpr (std::is_same_v<T, std::string>) {
                if (type == Type::FONT) return font;
            }
            return std::nullopt;
        }
};

#endif