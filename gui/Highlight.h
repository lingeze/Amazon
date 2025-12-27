#pragma once
#include "../src/position.h"
#include <vector>
#include <SFML/Graphics.hpp>
class Highlight{
    public:
    Highlight(float square_size);
    void update(Position select_pos,const std::vector<Position> &psb_move);
    void clear();
    void draw(sf::RenderWindow &window);
    void set_move_color(int id){
        if(id==1)highlight_psb_move.setFillColor(color1);
        else highlight_psb_move.setFillColor(color2);
    }
    Position get_pos();
    std::vector<Position> get_move();
    private:
    sf::Color color1=sf::Color(218, 165, 32, 135);
    sf::Color color2=sf::Color(173, 255, 47, 120);
    float square_size;
    sf::CircleShape highlight_select_pos;
    sf::RectangleShape highlight_psb_move;
    Position select_pos;
    std::vector<Position> psb_move;
};