#pragma once
#include <SFML/Graphics.hpp>
class PieceDrawer{
    public:
    PieceDrawer(float s_s);
    void draw(sf::RenderWindow &window,int piecetype,int row,int col);
    private:
    sf::CircleShape pieceshape;
    float square_size;
};