#pragma once
#include <SFML/Window.hpp>

class Input {
    static sf::Keyboard keyboard;
    public:
        static bool isKeyPressed(sf::Keyboard::Key key);
};