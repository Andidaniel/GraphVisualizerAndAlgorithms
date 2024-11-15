#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <string>
//TODO: MOVE THIS INTO SEPARATE FILE
class Menu {
public:
    Menu(float width, float height) {

        loadProjectFonts();

        // Define menu options
        menuItems.push_back("Start Drawing");
        menuItems.push_back("Test");
        menuItems.push_back("Exit");

        // Configure each menu text item
        for (size_t i = 0; i < menuItems.size(); i++) {
            sf::Text text;
            text.setFont(font);
            text.setString(menuItems[i]);
            text.setCharacterSize(24);
            text.setPosition(width / 2 - 50, height / (menuItems.size() + 1) * (i + 1));
            if (i == 0) {
                text.setFillColor(sf::Color::Green); // Highlight the first item
            }
            else {
                text.setFillColor(sf::Color::White);
            }
            menuText.push_back(text);
        }

        selectedItemIndex = 0;
    }

    std::string removeLastThreeFoldersFromFilepath(const std::string& path) {
        std::string modifiedPath = path;

        // Iterate three times to remove three folders
        for (int i = 0; i < 3; ++i) {
            // Find the last directory separator
            size_t pos = modifiedPath.find_last_of("/\\");

            // If found, remove everything after the last separator
            if (pos != std::string::npos) {
                modifiedPath = modifiedPath.substr(0, pos);
            }
            else {
                // If there aren't enough folders, we stop early
                break;
            }
        }

        return modifiedPath;
    }

    std::string ConstructResourcePath(const std::string& resourceTypeFolder, const std::string& resource)
    {
        //Get Executable Full Path
        WCHAR executablePath[MAX_PATH];;
        GetModuleFileName(NULL, executablePath, sizeof(executablePath)); 
        // Convert to wstring
        std::wstring executablePathWString = executablePath;
        // Convert wstring path to string for SFML (since SFML loadFromFile uses std::string)
        std::string executablePathString(executablePathWString.begin(), executablePathWString.end());
        //Get the path of the main project folder
        std::string mainProjectFolderPath = removeLastThreeFoldersFromFilepath(executablePathString);

        const std::string resourcesFolderName = "Resources";
        
        return mainProjectFolderPath + '\\' + resourcesFolderName + '\\' + resourceTypeFolder + '\\' + resource; 
    }

    void loadProjectFonts() {
        
        std::string minecraftFontPath = ConstructResourcePath("Fonts", "MINECRAFT_PE.ttf");

        // Menu items setup
        if (!font.loadFromFile(minecraftFontPath)) {
            // Handle error
            std::cerr << "ERROR: Couldn't find font file at :" + minecraftFontPath;
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& text : menuText) {
            window.draw(text);
        }
    }

    void moveUp() {
        if (selectedItemIndex - 1 >= 0) {
            menuText[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex--;
            menuText[selectedItemIndex].setFillColor(sf::Color::Green);
        }
    }

    void moveDown() {
        if (selectedItemIndex + 1 < menuItems.size()) {
            menuText[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex++;
            menuText[selectedItemIndex].setFillColor(sf::Color::Green);
        }
    }

    int getSelectedItemIndex() const {
        return selectedItemIndex;
    }

private:
    std::vector<sf::Text> menuText;
    std::vector<std::string> menuItems;
    sf::Font font;
    int selectedItemIndex;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Menu");

    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    int selectedItem = menu.getSelectedItemIndex();
                    // Handle menu selection
                    if (selectedItem == 0) {
                        // Start Game
                    }
                    else if (selectedItem == 1) {
                        // Options
                    }
                    else if (selectedItem == 2) {
                        window.close(); // Exit
                    }
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}