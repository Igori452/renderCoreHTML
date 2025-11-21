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

        enum class Type { UNDEFINED, DISPLAYVALUE, DOUBLE, COLOR };

        enum class DisplayType { INLINE, BLOCK, INLINE_BLOCK };

    private:
        Type type {Type::UNDEFINED};

        LengthUnit lengthUnit;

        DisplayType displayType;
        double numericValue;
        uint32_t colorValue;

        static bool parseColor(const std::string& stringColor,  uint32_t& color);
        
        static StyleValue setStyle();
        static StyleValue setStyle(DisplayType _value);
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
                if (type == Type::DISPLAYVALUE) return displayType;
            } else if constexpr (std::is_same_v<T, double>) {
                if (type == Type::DOUBLE) return numericValue;
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                if (type == Type::COLOR) return colorValue;
            }
            return std::nullopt;
        }

        // Проверки типа для удобства
        bool isDisplayType() const;
        bool isNumeric() const;
        bool isColor() const;
        bool isUndefined() const;
};

#endif