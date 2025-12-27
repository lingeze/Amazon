#include"BoardDrawer.h"
#include <iostream>
BoardDrawer::BoardDrawer(int b_s,float s_z):board_size(b_s),square_size(s_z){

}
void BoardDrawer::draw(sf::RenderWindow &window)const{
    sf::RectangleShape shape({square_size, square_size}); 
    for(int i=0;i<board_size;i++){
        for(int j=0;j<board_size;j++){
            if((i+j)%2==0){
                shape.setFillColor(color1);
            }
            else shape.setFillColor(color2);
            float shape_x=j*square_size;
            float shape_y=i*square_size;
            shape.setPosition({shape_x,shape_y});
            window.draw(shape);
        }
    }
}