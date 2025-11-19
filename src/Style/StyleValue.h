#ifndef STYLE_VALUE_H
#define STYLE_VALUE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include "StyleProperties.h"

class StyleValue {
    public:
        enum class LengthUnit { UNDEFINED, PX };

        enum class Type { UNDEFINED, DISPLAYVALUE, BORDERTYPE, DOUBLE, COLOR };

        enum class DisplayType { INLINE, BLOCK };
        enum class BorderType { NONE, SOLID, DASHED };

    private:
        Type type {Type::UNDEFINED};
        LengthUnit lengthUnit;

        DisplayType displayType;
        BorderType borderType;
        double numericValue;
        uint32_t colorValue;

        static bool parseColor(const std::string& stringColor,  uint32_t& color);
        
        static StyleValue setStyle();
        static StyleValue setStyle(DisplayType _value);
        static StyleValue setStyle(BorderType _value);
        static StyleValue setStyle(double _value, LengthUnit lengthUnit_);
        static StyleValue setStyle(uint32_t _value);

        static LengthUnit getLengthUnitFromString(const std::string& LengthUnitString);
    
    public:
        static StyleValue setStyleValueFromString(StyleProperty property, const std::string& stringValue, const std::string& lengthUnitString_ = "");
};

#endif