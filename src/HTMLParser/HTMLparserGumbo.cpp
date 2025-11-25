#include "HTMLparserGumbo.h"
#include "../Node/ElementNode.h"
#include "../Node/TextElement.h"

HTMLparserGumbo::HTMLparserGumbo() : HTMLparser() {}
HTMLparserGumbo::HTMLparserGumbo(std::string _filePath) : HTMLparser(_filePath) {}

Node* HTMLparserGumbo::buildNode() {
    GumboOutput* output = gumbo_parse(HTMLparser::getHtmlContent().c_str());
    GumboNode* rootGumbo = output->root;

    if (!rootGumbo || rootGumbo->type != GUMBO_NODE_ELEMENT) {
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        return nullptr;
    }

    // Очередь: (GumboNode*, Node*) — (узел Gumbo, соответствующий родитель)
    std::queue<std::pair<GumboNode*, Node*>> q;

    // Создаём корневой элемент
    ElementNode* rootNode = new ElementNode(gumbo_normalized_tagname(rootGumbo->v.element.tag));
    rootNode->setParent(nullptr);

    // Добавляем всех детей корня в очередь
    const GumboVector* rootChildren = &rootGumbo->v.element.children;
    for (unsigned int i = 0; i < rootChildren->length; ++i) {
        GumboNode* child = static_cast<GumboNode*>(rootChildren->data[i]);
        q.push({child, rootNode});
    }

    // BFS построение дерева
    while (!q.empty()) {
        auto [gNode, parentNode] = q.front();
        q.pop();

        // ЭЛЕМЕНТ
        if (gNode->type == GUMBO_NODE_ELEMENT) {

            ElementNode* elem = new ElementNode(gumbo_normalized_tagname(gNode->v.element.tag));
            elem->setParent(parentNode);
            parentNode->addChild(elem);

            // читаем атрибуты
            const GumboVector* attrs = &gNode->v.element.attributes;
            for (unsigned int i = 0; i < attrs->length; ++i) {
                const GumboAttribute* a =
                    static_cast<GumboAttribute*>(attrs->data[i]);
                elem->setAttribute(a->name, a->value);
            }

            // ставим детей в очередь
            const GumboVector* ch = &gNode->v.element.children;
            for (unsigned int i = 0; i < ch->length; ++i) {
                GumboNode* gumboChild = static_cast<GumboNode*>(ch->data[i]);
                q.push({gumboChild, elem});
            }
        }

        // ТЕКСТ
        else if (gNode->type == GUMBO_NODE_TEXT) {

            std::string text = gNode->v.text.text;

            // trim пробелов
            std::string trimmed;
            bool inSpace = false;
            for (char c : text) {
                if (isspace(c)) {
                    if (!inSpace) {
                        trimmed.push_back(' ');
                        inSpace = true;
                    }
                } else {
                    inSpace = false;
                    trimmed.push_back(c);
                }
            }
            if (!trimmed.empty() && trimmed.front() == ' ') trimmed.erase(trimmed.begin());
            if (!trimmed.empty() && trimmed.back() == ' ') trimmed.pop_back();

            if (!trimmed.empty()) {
                TextElement* t = new TextElement(trimmed);
                t->setParent(parentNode);
                parentNode->addChild(t);
            }
        }
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    applyMetricsRecursive(rootNode);

    return rootNode;
}

void HTMLparserGumbo::applyMetricsRecursive(Node* node) {
    if (!node) return;

    // Если это текстовый узел — применяем метрики
    if (node->getType() == Node::Type::TEXT_NODE) {
        TextElement* text = static_cast<TextElement*>(node);
        text->applyMetrics();
    }

    // Рекурсивно обходим всех детей
    for (Node* child : node->getChildren()) {
        applyMetricsRecursive(child);
    }
}