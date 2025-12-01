#include "StyleValue.h"

StyleValue StyleValue::setStyle() {
    StyleValue styleValue;
    styleValue.type = Type::UNDEFINED;
    return styleValue;
}

StyleValue StyleValue::setStyle(DisplayType _value) {
    StyleValue styleValue;
    styleValue.type = Type::DISPLAY;
    styleValue.displayType = _value;
    return styleValue;
}

StyleValue StyleValue::setStyle(FontStyleType _value) {
    StyleValue styleValue;
    styleValue.type = Type::FONT_STYLE;
    styleValue.fontStyleType = _value;
    return styleValue;
}

StyleValue StyleValue::setStyle(std::string _value) {
    StyleValue styleValue;
    styleValue.type = Type::STRINGVALUE;
    styleValue.stringValue = _value;
    return styleValue;
}

StyleValue StyleValue::setStyle(FontWeightType _value) {
    StyleValue styleValue;
    styleValue.type = Type::FONT_WEIGHT;
    styleValue.fontWeightType = _value;
    return styleValue;
}

StyleValue StyleValue::setStyle(double _value, LengthUnit lengthUnit_) {
    StyleValue styleValue;
    styleValue.type = Type::DOUBLE;
    styleValue.numericValue = _value;

    styleValue.lengthUnit = lengthUnit_;
    
    return styleValue;
}

StyleValue StyleValue::setStyle(uint32_t _value) {
    StyleValue styleValue;
    styleValue.type = Type::COLOR;
    styleValue.colorValue = _value;
    return styleValue;
}

bool StyleValue::parseColor(const std::string& stringColor, uint32_t& color) {
    static const std::unordered_map<std::string, uint32_t> colors = {
        {"black", 0x000000FF}, 
        {"white", 0xFFFFFFFF},
        {"red", 0xFF0000FF}, 
        {"green", 0x00FF00FF}, 
        {"blue", 0x0000FFFF},
        {"yellow", 0xFFFF00FF}, 
        {"cyan", 0x00FFFFFF}, 
        {"magenta", 0xFF00FFFF},
        {"gray", 0x808080FF}, 
        {"transparent", 0x00000000}
    };
    
    auto it = colors.find(stringColor);
    if (it != colors.end()) {
        color = it->second;
        return true;
    }
    
    if (stringColor[0] == '#' && stringColor.length() == 7) {
        color = std::stoul(stringColor.substr(1), nullptr, 16) << 8 | 0xFF;
        return true;
    }
    
    return false;
}

StyleValue::LengthUnit StyleValue::getLengthUnitFromString(const std::string& LengthUnitString) {
    static const std::unordered_map<std::string, LengthUnit> LengthUnitMap = {
        {"px", LengthUnit::PX},
    };

    auto it = LengthUnitMap.find(LengthUnitString);
    return it != LengthUnitMap.end() ? it->second : LengthUnit::UNDEFINED;
}

StyleValue StyleValue::setStyleValueFromString (StyleProperty property, const std::string& stringValue, const std::string& lengthUnitString_) {

    switch (property) {

        case StyleProperty::DISPLAY: {
            if (stringValue == "inline") return setStyle(DisplayType::INLINE);
            else if (stringValue == "block") return setStyle(DisplayType::BLOCK);
            else return setStyle();
        }

        case StyleProperty::BACKGROUND_IMAGE:
        case StyleProperty::FONT_FAMILY: {
            return setStyle(stringValue);
        }

        case StyleProperty::FONT_STYLE: {
            if (stringValue == "normal") return setStyle(FontStyleType::NORMAL);
            else if (stringValue == "italic") return setStyle(FontStyleType::ITALIC);
            else if (stringValue == "underlined") return setStyle(FontStyleType::UNDERLINED);
            else return setStyle();
        }

        case StyleProperty::FONT_WEIGHT: {
            if (stringValue == "normal") return setStyle(FontWeightType::NORMAL);
            else if (stringValue == "bold") return setStyle(FontWeightType::BOLD);
            else return setStyle();
        }

        case StyleProperty::COLOR:
        case StyleProperty::BACKGROUND_COLOR:
        case StyleProperty::BORDER_COLOR: {
            uint32_t color;
            bool status = parseColor(stringValue, color);
            if (status) return setStyle(color);
            else return setStyle();
        }

        case StyleProperty::FONT_SIZE:
        case StyleProperty::WIDTH:
        case StyleProperty::HEIGHT:
        case StyleProperty::BORDER_WIDTH:
        case StyleProperty::MARGIN_TOP:
        case StyleProperty::MARGIN_RIGHT:
        case StyleProperty::MARGIN_BOTTOM:
        case StyleProperty::MARGIN_LEFT:
        case StyleProperty::PADDING_TOP:
        case StyleProperty::PADDING_RIGHT:
        case StyleProperty::PADDING_BOTTOM:
        case StyleProperty::PADDING_LEFT: {

            LengthUnit lengthUnit = LengthUnit::UNDEFINED;
            if (!lengthUnitString_.empty()) lengthUnit = getLengthUnitFromString(lengthUnitString_);

            try {
                double numericValue = std::stod(stringValue);
                return setStyle(numericValue, lengthUnit);
            } catch (...) {
                return setStyle();
            }
        }
        default:
            return setStyle();
    }
}

StyleValue::DisplayType StyleValue::getDisplayType() const {
    if (type != Type::DISPLAY) {
        return DisplayType::BLOCK;
    }
    return displayType;
}

double StyleValue::getNumericValue() const {
    if (type != Type::DOUBLE) {
        return 0.0;
    }
    return numericValue;
}

uint32_t StyleValue::getColorValue() const {
    if (type != Type::COLOR) {
        return 0;
    }
    return colorValue;
}

StyleValue::LengthUnit StyleValue::getLengthUnit() const {
    return lengthUnit;
}

StyleValue::Type StyleValue::getType() const {
    return type;
}