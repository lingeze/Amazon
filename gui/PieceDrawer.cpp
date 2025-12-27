#include "PieceDrawer.h"
#include<iostream>
PieceDrawer::PieceDrawer(float s_s):square_size(s_s){
    float R=square_size*0.4f;
    pieceshape.setRadius(R);
    pieceshape.setOrigin({R,R});
}
void PieceDrawer::draw(sf::RenderWindow &window,int piecetype,int row,int col){
    if(piecetype==1){
        pieceshape.setFillColor(sf::Color(80, 80, 80));
    }
    else if(piecetype==-1){
        pieceshape.setFillColor(sf::Color::White);
        pieceshape.setOutlineColor(sf::Color::Black);
        pieceshape.setOutlineThickness(1);
    }
    else if(piecetype==2){
        pieceshape.setFillColor(sf::Color::Blue);
        pieceshape.setOutlineColor(sf::Color::Black);
        pieceshape.setOutlineThickness(1);
    }
    else return;
    float y=square_size/2+row*square_size;
    float x=square_size/2+col*square_size;
    pieceshape.setPosition({x,y});
    window.draw(pieceshape);
}