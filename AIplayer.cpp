#include "AIplayer.h"
#include <random>
AIplayer::AIplayer(int color):player_color(color),gen(std::random_device()()){
    
}
Move AIplayer::get_move(const Board &now_board)const{
    return rand_strategy(now_board);
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