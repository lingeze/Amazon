#include "move.h"
#include <iostream>
Move::Move(){

}
Move::Move(const Position &initial_begin, const Position &initial_end,
const Position &initial_obstacle):
begin(initial_begin),end(initial_end),obstacle(initial_obstacle){
    
}
void Move::printmove()const{
    std::cout<<"起点:"<<begin.y<<" "<<begin.x<<std::endl;
    std::cout<<"终点:"<<end.y<<" "<<end.x<<std::endl;
    std::cout<<"障碍物:"<<obstacle.y<<" "<<obstacle.x<<std::endl;
}
