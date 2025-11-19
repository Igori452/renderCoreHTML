#include "HTMLparserGumbo.h"
#include "../Node/ElementNode.h"
#include "../Node/TextElement.h"

HTMLparserGumbo::HTMLparserGumbo() : HTMLparser() {}
HTMLparserGumbo::HTMLparserGumbo(std::string _filePath) : HTMLparser(_filePath) {}

Node* HTMLparserGumbo::buildNode () {
    GumboOutput* output = gumbo_parse(HTMLparser::getHtmlContent().c_str());
    
    std::queue<std::pair<GumboNode*, Node*>> queueNodes;
    
    GumboNode* currentNode = output->root;

    bool root = true;
    Node* rootNode = nullptr;

    do {
        if (currentNode->type == GUMBO_NODE_ELEMENT) {
            ElementNode* node = new ElementNode(gumbo_normalized_tagname(currentNode->v.element.tag));

            const GumboVector* attributes = &currentNode->v.element.attributes;
            for (unsigned int i = 0; i < attributes->length; ++i) {
                const GumboAttribute* attribute = static_cast<GumboAttribute*>(attributes->data[i]);
                node->setAttribute(attribute->name, attribute->value);
            }

            const GumboVector* children = &currentNode->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i) {
                GumboNode* child = static_cast<GumboNode*>(children->data[i]);
                queueNodes.push(std::make_pair(child, node));
            }

            if (root) {
                rootNode = node;
                root = false;
            } else queueNodes.front().second->addChild(node); 

        } else if (currentNode->type == GUMBO_NODE_TEXT && !root) 
            queueNodes.front().second->addChild(new TextElement(currentNode->v.text.text)); 
        
        queueNodes.pop();
        currentNode = queueNodes.front().first;
        
    } while (!queueNodes.empty());

    return rootNode;
}