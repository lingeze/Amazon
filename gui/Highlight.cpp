#include "Highlight.h"
#include <iostream>
Highlight::Highlight(float square_size):square_size(square_size){
    select_pos={-1,-1};
    float R=square_size*0.4f;
    highlight_select_pos.setRadius(R);
    highlight_select_pos.setOrigin({R,R});
    highlight_select_pos.setFillColor(sf::Color(60, 179, 113, 180));
    highlight_psb_move.setSize({square_size,square_size});
    highlight_psb_move.setFillColor(color1);
}   
void Highlight::update(Position select_pos, const std::vector<Position> &psb_move){
    this->select_pos=select_pos;
    this->psb_move=psb_move;
}
void Highlight::clear(){
    select_pos={-1,-1};
    psb_move.clear();
}
void Highlight::draw(sf::RenderWindow &window){
    if(select_pos.x!=-1){
        std::cout<<select_pos.x<<" "<<select_pos.y<<std::endl;
        float x=select_pos.y*square_size+square_size/2.0f;
        float y=select_pos.x*square_size+square_size/2.0f;
        highlight_select_pos.setPosition({x,y});
        window.draw(highlight_select_pos);
    }
    for(auto cur:psb_move){
        float x=cur.y*square_size;
        float y=cur.x*square_size;
        highlight_psb_move.setPosition({x,y});
        window.draw(highlight_psb_move);
    }
}
Position Highlight::get_pos(){
    return select_pos;
}
std::vector<Position> Highlight::get_move(){
    return psb_move;
}
