#include <iostream>
#include "HTMLparser/HTMLparserGumbo.h"
#include "Node/Node.h"

int main () {
    
    HTMLparserGumbo html("../htmlTests/test.html");

    Node* rootNode = html.parse();
    rootNode->printNodes();

    std::cout << std::endl;

    return 0;
}