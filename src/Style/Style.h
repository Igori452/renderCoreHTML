#ifndef STYLE_H
#define STYLE_H

#include "StyleValue.h"

class Style {
    private:
        std::unordered_map<StyleProperty, StyleValue> properties;

        // Общие значения по умолчанию хранящиеся в статической памяти
        // (одинаковые для всех классов)
        const static std::unordered_map<StyleProperty, StyleValue> propertiesDefault;

        StyleProperty stringToProperty(const std::string& propertyName);
    public:
        bool setProperty(const std::string& property, const std::string& valueProperty, const std::string& lengthValue = "");
        const std::unordered_map<StyleProperty, StyleValue>& getMapProperty() const;
        
        //bool apply(std::map<std::string, std::string>);
        //void parseMap(std::string styles) 
};

#endif