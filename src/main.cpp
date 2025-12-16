#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "Node/Node.h"
#include "Layout/LayoutBox.h"
#include "Layout/LayoutEngine.h"
#include "Render/RendererSFML.h"
#include "HTMLParser/HTMLparserGumbo.h"

namespace fs = std::filesystem;

void runTest(const std::string& filePath) {
    std::cout << "Running test: " << filePath << std::endl;
    
    HTMLparserGumbo html(filePath);
    Node* rootNode = html.parse();
    
    LayoutEngine* layoutEngine = new LayoutEngine();
    RendererSFML* renderer = new RendererSFML(layoutEngine->getWindowWidth(), layoutEngine->getWindowHeight());
    
    LayoutBox* rootLayoutBox = layoutEngine->buildLayoutTree(rootNode);
    layoutEngine->computeLayout(rootLayoutBox, layoutEngine->getWindowWidth(), layoutEngine->getWindowHeight());

    renderer->showScene(*rootLayoutBox);
    
    // Cleanup
    delete layoutEngine;
    delete renderer;
}

int main() {
    // Список доступных тестов
    std::vector<std::string> tests = {
        "../htmlTests/basic_styles.html",
        "../htmlTests/inheritance_test.html", 
        "../htmlTests/clipping_test.html",
        "../htmlTests/text_wrapping.html",
        "../htmlTests/block_inline_test.html",
        "../htmlTests/margins_padding_test.html",
        "../htmlTests/display_none_test.html",
        "../htmlTests/complex_test.html",
        "../htmlTests/images_backgrounds_test.html",
        "../htmlTests/minimal_test.html"
    };
    
    std::vector<std::string> testNames = {
        "Basic Styles",
        "Inheritance Test",
        "Clipping Test", 
        "Text Wrapping",
        "Block & Inline Elements",
        "Margins & Padding",
        "Display: None",
        "Complex Test",
        "Images & Backgrounds",
        "Minimal Test"
    };
    
    while (true) {
        std::cout << "\n=== Render Engine Test Menu ===" << std::endl;
        std::cout << "Available tests:" << std::endl;
        
        for (size_t i = 0; i < testNames.size(); ++i) {
            std::cout << i + 1 << ". " << testNames[i] 
                     << " (" << fs::path(tests[i]).filename().string() << ")" << std::endl;
        }
        
        std::cout << "11. Custom HTML file (current folder ../htmlTest/)" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 0) {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        
        if (choice == 11) {
            std::cout << "Enter full path to HTML file: ";
            std::string customPath;
            std::cin.ignore();
            std::getline(std::cin, customPath);
            customPath = "../htmlTests/" + customPath;
            if (fs::exists(customPath)) {
                try {
                    runTest(customPath);
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            } else {
                std::cout << "File not found: " << customPath << std::endl;
            }
        }
        else if (choice >= 1 && choice <= 10) {
            size_t index = choice - 1;
            
            if (fs::exists(tests[index])) {
                try {
                    runTest(tests[index]);
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            } else {
                std::cout << "Test file not found: " << tests[index] << std::endl;
            }
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }
    
    return 0;
}