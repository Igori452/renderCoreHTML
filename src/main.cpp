#include <iostream>
#include "Node/Node.h"
#include "Layout/LayoutBox.h"
#include "Layout/LayoutEngine.h"
#include "Render/RendererSFML.h"
#include "HTMLParser/HTMLparserGumbo.h"

int main () {
    HTMLparserGumbo html("../htmlTests/test.html");

    Node* rootNode = html.parse();

    std::cout << std::endl << std::endl;

    LayoutEngine* layoutEngine = new LayoutEngine();
    RendererSFML* renderer = new RendererSFML(layoutEngine->getWindowWidth(), layoutEngine->getWindowHeight());

    LayoutBox* rootLayoutBox = layoutEngine->buildLayoutTree(rootNode);
    layoutEngine->computeLayout(rootLayoutBox, layoutEngine->getWindowWidth(), layoutEngine->getWindowHeight());

    rootLayoutBox->printLayoutBoxTree();
    std::cout << std::endl;

    renderer->showScene(*rootLayoutBox);

    return 0;
}