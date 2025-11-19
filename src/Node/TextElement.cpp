#include "TextElement.h"

TextElement::TextElement() : Node(), text("") {}
TextElement::TextElement(const char* _text) : Node(), text(_text) {}
TextElement::TextElement(std::string _text) : Node(), text(_text) {}

TextElement::Type TextElement::getType() const { return Type::TEXT_NODE; }

const std::string TextElement::getText() const { return text; }