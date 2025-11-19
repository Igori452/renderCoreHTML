#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <cctype>
#include <fstream>
#include <iostream> //?
#include "../Node/Node.h"

class HTMLparser {
    private:
        std::string htmlContent;
        std::string filePath;
        std::string error;

        virtual Node* buildNode() = 0;
        bool loadFile();
        void readFile();
    public:
        HTMLparser();
        HTMLparser(std::string _filePath);

        Node* parse();

        std::string getHtmlContent();
        std::string getFilePath();
        std::string getError();
};

#endif