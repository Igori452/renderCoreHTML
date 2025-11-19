#ifndef TEXT_ELEMENT_H
#define TEXT_ELEMENT_H

#include <string>
#include "Node.h"

class TextElement : public Node {
    private:
        std::string text;
    public:
        TextElement();
        TextElement(const char* _text);
        TextElement(std::string _text);

        Type getType() const override;

        const std::string getText() const;
};

#endif