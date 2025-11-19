#include "Style.h"

StyleProperty Style::stringToProperty(const std::string& propertyName) {
    // статический список поддерживаемых стилей
    static const std::unordered_map<std::string, StyleProperty> propertyMap = {
        {"display", StyleProperty::DISPLAY},
        {"width", StyleProperty::WIDTH},
        {"height", StyleProperty::HEIGHT},
        {"margin-top", StyleProperty::MARGIN_TOP},
        {"margin-right", StyleProperty::MARGIN_RIGHT},
        {"margin-bottom", StyleProperty::MARGIN_BOTTOM},
        {"margin-left", StyleProperty::MARGIN_LEFT},
        {"padding-top", StyleProperty::PADDING_TOP},
        {"padding-right", StyleProperty::PADDING_RIGHT},
        {"padding-bottom", StyleProperty::PADDING_BOTTOM},
        {"padding-left", StyleProperty::PADDING_LEFT},
        {"border-style", StyleProperty::BORDER_STYLE},
        {"border-width", StyleProperty::BORDER_WIDTH},
        {"border-color", StyleProperty::BORDER_COLOR},
        {"color", StyleProperty::COLOR},
        {"background-color", StyleProperty::BACKGROUND_COLOR}
    };

    auto it = propertyMap.find(propertyName);
    return it != propertyMap.end() ? it->second : StyleProperty::UNKNOWN;
}

bool Style::setProperty(const std::string& property, const std::string& valueProperty, const std::string& lengthValue) {
    StyleProperty propertyObject = stringToProperty(property);

    if (propertyObject == StyleProperty::UNKNOWN) return false;

    properties[propertyObject] = StyleValue::setStyleValueFromString(propertyObject, valueProperty, lengthValue);

    return true;
}

const std::unordered_map<StyleProperty, StyleValue>& Style::getMapProperty() {
    return properties;
}

const std::unordered_map<StyleProperty, StyleValue> Style::propertiesDefault = {
    // DISPLAY - строковое значение
    {StyleProperty::DISPLAY, StyleValue::setStyleValueFromString(StyleProperty::DISPLAY, "inline")},
    
    // Размеры - числовые значения с "px" по умолчанию
    {StyleProperty::WIDTH, StyleValue::setStyleValueFromString(StyleProperty::WIDTH, "0", "px")},
    {StyleProperty::HEIGHT, StyleValue::setStyleValueFromString(StyleProperty::HEIGHT, "0", "px")},
    
    // Внешние отступы - числовые значения с "px" по умолчанию
    {StyleProperty::MARGIN_TOP, StyleValue::setStyleValueFromString(StyleProperty::MARGIN_TOP, "0", "px")},
    {StyleProperty::MARGIN_RIGHT, StyleValue::setStyleValueFromString(StyleProperty::MARGIN_RIGHT, "0", "px")},
    {StyleProperty::MARGIN_BOTTOM, StyleValue::setStyleValueFromString(StyleProperty::MARGIN_BOTTOM, "0", "px")},
    {StyleProperty::MARGIN_LEFT, StyleValue::setStyleValueFromString(StyleProperty::MARGIN_LEFT, "0", "px")},
    
    // Внутренние отступы - числовые значения с "px" по умолчанию
    {StyleProperty::PADDING_TOP, StyleValue::setStyleValueFromString(StyleProperty::PADDING_TOP, "0", "px")},
    {StyleProperty::PADDING_RIGHT, StyleValue::setStyleValueFromString(StyleProperty::PADDING_RIGHT, "0", "px")},
    {StyleProperty::PADDING_BOTTOM, StyleValue::setStyleValueFromString(StyleProperty::PADDING_BOTTOM, "0", "px")},
    {StyleProperty::PADDING_LEFT, StyleValue::setStyleValueFromString(StyleProperty::PADDING_LEFT, "0", "px")},
    
    // Границы
    {StyleProperty::BORDER_STYLE, StyleValue::setStyleValueFromString(StyleProperty::BORDER_STYLE, "none")}, // строковое
    {StyleProperty::BORDER_WIDTH, StyleValue::setStyleValueFromString(StyleProperty::BORDER_WIDTH, "0", "px")}, // числовое с "px"
    {StyleProperty::BORDER_COLOR, StyleValue::setStyleValueFromString(StyleProperty::BORDER_COLOR, "black")}, // строковое
    
    // Цвета
    {StyleProperty::COLOR, StyleValue::setStyleValueFromString(StyleProperty::COLOR, "black")},
    {StyleProperty::BACKGROUND_COLOR, StyleValue::setStyleValueFromString(StyleProperty::BACKGROUND_COLOR, "transparent")},
};