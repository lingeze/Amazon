#pragma once
#include <SFML/Graphics.hpp>
class BoardDrawer{
    public:
    BoardDrawer(int b_s,float s_z);
    void draw(sf::RenderWindow &window)const;
    private:
    int board_size;//棋盘格子数
    float square_size;
    sf::Color color1=sf::Color(255, 255, 255);
    sf::Color color2=sf::Color(200, 200, 200);
};