#include "position.h"
Position::Position(): x(0),y(0){

}
Position::Position(int initial_x,int initial_y): x(initial_x),y(initial_y){

}
bool Position::operator==(const Position &other)const{
    return (x==other.x)&&(y==other.y);
} 
