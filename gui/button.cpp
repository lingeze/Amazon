#include "button.h"
void Button::setupButton(sf::Vector2f position, sf::Vector2f size, const sf::Font& font, 
                   const std::wstring& text, unsigned int charSize) {
    buttonShape.setPosition(position);
    buttonShape.setSize(size);
    buttonShape.setFillColor(sf::Color::Blue);
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(charSize);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.getCenter());
    buttonText.setPosition({position.x+size.x/2,position.y+size.y/2});
}
bool Button::isMouseOver(sf::Vector2f mousePos) {
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    return buttonBounds.contains(mousePos);
}
