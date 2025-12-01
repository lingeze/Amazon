#include "AIplayer.h"
#include <random>
#include <iostream>
AIplayer::AIplayer(int color):player_color(color),gen(std::random_device()()){
    
}
Move AIplayer::get_move(const Board &now_board)const{
    Move ret=rand_strategy(now_board);
    std::cout<<"起点:"<<ret.begin.x<<" "<<ret.begin.y<<std::endl;
    std::cout<<"终点:"<<ret.end.x<<" "<<ret.end.y<<std::endl;
    std::cout<<"障碍物:"<<ret.obstacle.x<<" "<<ret.obstacle.y<<std::endl;
    return ret;
}
Move AIplayer::rand_strategy(const Board &now_board)const{
    vector<Move> psb_move=now_board.get_all_psb_move(player_color);
    int len=psb_move.size();
    std::uniform_int_distribution<> distr(0,len-1);
    int rand_number=distr(gen);
    return psb_move[rand_number];
}
AIplayer::~AIplayer(){
    
}