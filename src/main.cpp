#include <iostream>
#include "HTMLparser/HTMLparserGumbo.h"
#include "Node/Node.h"
#include "Layout/LayoutBox.h"
#include "Layout/LayoutEngine.h"

#include <SFML/Graphics.hpp>

int main () {
    
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test - Mini Browser");
    
    if (!window.isOpen()) {
        std::cout << "❌ Failed to create SFML window!" << std::endl;
        return 1;
    }
    
    std::cout << "✅ SFML window created successfully!" << std::endl;
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
    
    // Создаем простую фигуру - красный круг
    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(300, 200);

    // Главный цикл (исправленный для SFML 2.5/2.6)
    while (window.isOpen())
    {
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Очищаем окно черным цветом
        window.clear(sf::Color::Black);

        // Рисуем круг
        window.draw(circle);

        // Отображаем кадр
        window.display();
    }

    HTMLparserGumbo html("../htmlTests/test.html");

    Node* rootNode = html.parse();
    rootNode->printNodes();

    std::cout << std::endl << std::endl;

    LayoutEngine* layoutEngine = new LayoutEngine();
    LayoutBox* rootLayoutBox = layoutEngine->buildLayoutTree(rootNode);
    layoutEngine->computeLayout(rootLayoutBox, layoutEngine->getWindowWidth(), layoutEngine->getWindowHeight());
    rootLayoutBox->printLayoutBoxTree();
    
    std::cout << std::endl;

    return 0;
}