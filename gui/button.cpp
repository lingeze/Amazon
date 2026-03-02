#include "button.h"
Button::Button(const sf::Font& font,const std::wstring& text,int charSize) :buttonText(font,text,charSize)
{

}
void Button::setupButton(sf::Vector2f position, sf::Vector2f size)
{
    buttonShape.setPosition(position);
    buttonShape.setSize(size);
    buttonShape.setFillColor(sf::Color::Cyan);
    buttonText.setCharacterSize(static_cast<unsigned int>(size.y*0.5f));
    buttonText.setFillColor(sf::Color::Black);
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.getCenter());
    buttonText.setPosition({position.x+size.x/2,position.y+size.y/2});
}
void Button::setuptext(const std::wstring &text){
    buttonText.setString(text);
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.getCenter());
}
void Button::setupbuttoncolor(sf::Color color){
    buttonShape.setFillColor(color);
}
void Button::setuptextcolor(sf::Color color)
{
    buttonText.setFillColor(color);
}
bool Button::isMouseOver(sf::Vector2f mousePos)
{
    sf::FloatRect buttonBounds = buttonShape.getGlobalBounds();
    return buttonBounds.contains(mousePos);
}
