#include <iostream>
#include "position.h"
#include "move.h"
#include "board.h"
#include "Humanplayer.h"
#include "AIplayer.h"
#include "filemanage.h"
int turn_id;
int main(){
    Board gameboard;
    gameboard.initialize();
    int my_color=-1;
    Move move;
    std::cin>>turn_id;
    std::cin>>move.begin.y>>move.begin.x>>move.end.y>>move.end.x>>move.obstacle.y>>move.obstacle.x;
    if(move.begin.y<0)my_color=1;
    else gameboard.make_move(move,-my_color);
    AIplayer ai(my_color);
    int now_color=my_color;
    for(int i=1;i<=turn_id-1;i++){
        std::cin>>move.begin.y>>move.begin.x>>move.end.y>>move.end.x>>move.obstacle.y>>move.obstacle.x;
        gameboard.make_move(move,now_color);
        if(i!=turn_id)
        std::cin>>move.begin.y>>move.begin.x>>move.end.y>>move.end.x>>move.obstacle.y>>move.obstacle.x;
        gameboard.make_move(move,-now_color);
    }
    Move nxt_move=ai.get_move(gameboard,my_color);
    std::cout<<nxt_move.begin.y<<" "<<nxt_move.begin.x<<" ";
    std::cout<<nxt_move.end.y<<" "<<nxt_move.end.x<<" ";
    std::cout<<nxt_move.obstacle.y<<" "<<nxt_move.obstacle.x<<" ";
}