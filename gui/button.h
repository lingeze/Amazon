#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
class Button{
    public:
    Button(const sf::Font& font,const std::wstring& text,int charSize);
    void setupButton(sf::Vector2f position, sf::Vector2f size);
    void setuptext(const std::wstring& text);
    void setupbuttoncolor(sf::Color color);
    void setuptextcolor(sf::Color color);
    void draw(sf::RenderWindow& window){
        window.draw(buttonShape);
        window.draw(buttonText);
    }
    bool isMouseOver(sf::Vector2f mousePos);
    private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};