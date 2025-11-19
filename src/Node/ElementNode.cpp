#include "ElementNode.h"

ElementNode::ElementNode() : Node() {};
ElementNode::ElementNode(std::string tagName) : Node(tagName) {};

void ElementNode::setAttribute (std::string attrName, std::string attrValue) {
    if (attrName == "style") {
        std::istringstream stream(attrValue);
        std::string property;
        std::map<std::string, std::string> cssProperties;

        while (std::getline(stream, property, ';')) {
            size_t colonPos = property.find(':');
            if (colonPos != std::string::npos) {
                std::string key = property.substr(0, colonPos);
                std::string value = property.substr(colonPos + 1);
                std::string lengthUnit = ""; 

                // Убираем лишние пробелы
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                if (!value.empty()) {
                    // Ищем позицию первого не-цифрового символа (и не точки, не минуса)
                    size_t i = 0;
                    while (i < value.length() && 
                          (std::isdigit(value[i]) || value[i] == '.' || value[i] == '-')) {
                        i++;
                    }
                    
                    // Если нашли цифры и после них есть текст - это единица измерения
                    if (i > 0 && i < value.length()) {
                        lengthUnit = value.substr(i);  // сохраняем единицу
                        value = value.substr(0, i);    // обрезаем значение
                    }
                }

                style.setProperty(key, value, lengthUnit);                
            }
        }
    }
    
    attributes.insert({attrName, attrValue});
}

ElementNode::Type ElementNode::getType() const { return Type::ELEMENT_NODE; }

const std::map<std::string, std::string>& ElementNode::getAttributes() const {
    return attributes;
}

const Style& ElementNode::getStyle() const {
    return style;
}