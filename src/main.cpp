#include <iostream>
#include "Node/Node.h"
#include "Layout/LayoutBox.h"
#include "Layout/LayoutEngine.h"
#include "Render/RendererSFML.h"
#include "HTMLParser/HTMLparserGumbo.h"

int main () {

    #include "Text/TextMetrics.h"

    std::cout << "SFML is using DISPLAY=" << getenv("DISPLAY") << std::endl;

    TextMetrics text;
    text.setFontFromFile("../../fonts/arialmt.ttf");

    HTMLparserGumbo html("../htmlTests/test.html");

    Node* rootNode = html.parse();
    rootNode->printNodes();

    std::cout << std::endl << std::endl;

    LayoutEngine* layoutEngine = new LayoutEngine();
    LayoutBox* rootLayoutBox = layoutEngine->buildLayoutTree(rootNode);
    layoutEngine->computeLayout(rootLayoutBox, layoutEngine->getWindowWidth(), layoutEngine->getWindowHeight());
    rootLayoutBox->printLayoutBoxTree();
    
    std::cout << std::endl;

    RendererSFML* render = new RendererSFML();
    render->showScene(*rootLayoutBox);

    return 0;
}