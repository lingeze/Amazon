#include "Humanplayer.h"
#include <iostream>
Humanplayer::Humanplayer(int color):player_color(color){

}
Move Humanplayer::get_move(const Board &now_board)const{
    now_board.print_board();
    Move ret_move={{0,0},{0,0},{0,0}};
    Position begin,end,obstacle;
    int cnt=0;
    while(!now_board.check_move(ret_move,player_color)){
        if(cnt)std::cout<<"您输入的行动不合法，请重新输入！"<<std::endl;
        cnt++;
        std::cout<<"请输入要移动的棋子的初始坐标："<<std::endl;
        std::cin>>begin.x>>begin.y;
        std::cout<<"请输入棋子移动后的坐标："<<std::endl;
        std::cin>>end.x>>end.y;
        std::cout<<"请输入释放障碍物的坐标："<<std::endl;
        std::cin>>obstacle.x>>obstacle.y;
        ret_move.begin=begin;
        ret_move.end=end;
        ret_move.obstacle=obstacle;
    }
    return ret_move;
}
Humanplayer::~Humanplayer(){
    
}