#pragma once
#include <SFML/Window.hpp>
#include <map>

class Input {
    static sf::Keyboard keyboard;
    static std::map<std::string, sf::Keyboard::Key> keyMap;
    public:
        static bool isKeyPressed(sf::Keyboard::Key key);
        static bool isActionPressed(std::string action);
        static void mapAction(std::string action, sf::Keyboard::Key key);
};