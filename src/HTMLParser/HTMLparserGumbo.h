#ifndef HTML_PARSER_GUMBO_H
#define HTML_PARSER_GUMBO_H

#include "HTMLparser.h"
#include <iostream>
#include <queue>
#include "gumbo.h"

class HTMLparserGumbo : public HTMLparser {
    private:
        Node* buildNode() override;
    public:
        HTMLparserGumbo();
        HTMLparserGumbo(std::string _filePath);
        void applyMetricsRecursive(Node* node);
};

#endif