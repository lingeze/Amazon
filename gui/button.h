#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
class Button{
    public:
    void setupButton(sf::Vector2f position, sf::Vector2f size, const sf::Font& font, 
                   const std::wstring& text, unsigned int charSize);
    void draw(sf::RenderWindow& window){
        window.draw(buttonShape);
        window.draw(buttonText);
    }
    bool isMouseOver(sf::Vector2f mousePos);
    private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;     
};