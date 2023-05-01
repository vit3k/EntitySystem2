#include <iostream>
#include "Input.h"

bool Input::isKeyPressed(sf::Keyboard::Key key)
{
    return keyboard.isKeyPressed(key);
}

bool Input::isActionPressed(std::string action) {
    //std::cout << "check if action pressed" << std::endl;
    //return keyboard.isKeyPressed(keyMap[action]);
    auto pressed = keyboard.isKeyPressed(keyMap[action]);
    //std::cout << pressed << std::endl;
    return pressed;
}

void Input::mapAction(std::string action, sf::Keyboard::Key key) {
    std::cout << "mapping action " << action << " " << key << std::endl;
    keyMap[action] = key;
}

std::map<std::string, sf::Keyboard::Key> Input::keyMap;
sf::Keyboard Input::keyboard;

