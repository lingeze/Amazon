#include "Humanplayer.h"
#include <iostream>
#include <string>
#include <sstream>
Humanplayer::Humanplayer(int color):player_color(color){

}
Position Humanplayer::to_position(std::string x,std::string y)const{
    //std::cout<<x<<" "<<x.size()<<" "<<y<<" "<<y.size()<<std::endl;
    if(x.size()!=1||y.size()!=1)throw std::runtime_error("您的输入有误，请按照给定格式输入。");
    if(!isdigit(x[0])||!isdigit(y[0]))throw std::runtime_error("您的输入不是数字，请按照给定格式输入。");
    int numx=x[0]-'0',numy=y[0]-'0';
    return {numx,numy};
}
Move Humanplayer::get_move(std::string user_input,const Board &now_board)const{
   // now_board.print_board();
    std::stringstream ss(user_input);
    Move ret_move={{0,0},{0,0},{0,0}};
    Position begin,end,obstacle;
    std::string bgx,bgy,edx,edy,obx,oby;
    ss>>bgy>>bgx>>edy>>edx>>oby>>obx;
    //std::cout<<bgy<<bgx<<edy<<edx<<oby<<obx<<std::endl;
    if(bgx.empty()||bgy.empty()||edx.empty()||edy.empty()||obx.empty()||oby.empty()){
        throw std::runtime_error("输入过少，请输入三个坐标。");
    }
    std::string junk;
    ss>>junk;
    if(!junk.empty()){
        throw std::runtime_error("输入过多，请输入三个坐标。");
    }
    try{
        ret_move.begin=to_position(bgx,bgy);
        ret_move.end=to_position(edx,edy);
        ret_move.obstacle=to_position(obx,oby);
    }catch(const std::exception &e){
        throw;
    }
    if(!now_board.check_move(ret_move,player_color)){
        throw std::runtime_error("输入的行动不符合规则，请重新输入。");
    }
    return ret_move;
}
Humanplayer::~Humanplayer(){
    
}