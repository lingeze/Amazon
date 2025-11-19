#include<iostream>
#include "move.h"
#include "board.h"
using namespace std;
void move_test(){
    Position start(1,0);
    Position end(1,2);
    Position ob(3,3);
    Move move(start,end,ob);
    cout<<move.begin.x<<" "<<move.begin.y<<endl;
    cout<<move.end.x<<" "<<move.end.y<<endl;
    cout<<move.obstacle.x<<" "<<move.obstacle.y<<endl;
}
void board_test(){
    Board bd;
    bd.print_board();
    //vector<vector<int>> grid=bd.get_grid();
    bd.add({1,1},1);
    bd.print_board();
    bd.add({3,3},1);
    bd.print_board();
    bd.add({1,7},-1);
    bd.print_board();
    Move move={{0,0},{0,2},{2,0}};
    std::cout<<bd.check_move(move,1);
    bd.add({0,0},1);
    std::cout<<bd.check_move(move,1);
    std::cout<<bd.check_move(move,-1);
    move.obstacle={4,4};
    std::cout<<bd.check_move(move,1);
    move.obstacle={4,2};
    std::cout<<bd.check_move(move,1);
    move.obstacle={6,8};
    std::cout<<bd.check_move(move,1);
    bd.del({1,1});
    bd.print_board();
    bd.del({1,7});
    bd.print_board();

}
int main(){
    board_test();    return 0;
}