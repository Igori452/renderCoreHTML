#include "HTMLparser.h"

HTMLparser::HTMLparser() : htmlContent("none"), filePath("none"), error("none") {}

HTMLparser::HTMLparser(std::string _filePath) : error("none") {
    filePath = _filePath;
    if (loadFile()) readFile();
    else htmlContent = "none";
}

bool HTMLparser::loadFile() {
    std::string extension = filePath.substr(filePath.find_last_of("."));
    for (char& c : extension) c = std::tolower(c);
    
    if (extension != ".html") {
        error = "Error! Invalid file extension: " + extension; 
        return false;
    }

    std::fstream file(filePath);
    if (!file.is_open()) {
        error = "Error! Cannot open file: " + filePath;
        return false;
    }
    file.close();
    
    std::cout << "The file was successfully opened!" << std::endl;
    return true;
}

void HTMLparser::readFile() {
    std::fstream file(filePath);

    std::string line;
    while (std::getline(file, line)) htmlContent += line;
}
 
Node* HTMLparser::parse() {
    if (error == "none") return buildNode();

    std::cout << error << std::endl;
    return nullptr;
}

std::string HTMLparser::getHtmlContent() { return htmlContent; }

std::string HTMLparser::getFilePath() { return filePath; }

std::string HTMLparser::getError() { return error; }