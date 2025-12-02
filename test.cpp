#include<iostream>
#include "move.h"
#include "board.h"
#include "AIplayer.h"
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
    AIplayer AI(-1);
    bd.initialize();
    bd.print_board();
    //bd.make_move({{2,0},{2,2},{3,3}},1);
    bd.print_board();
    vector<Move> move=bd.get_all_psb_move(-1);
     for(auto mv:move){
        if(mv.begin.y==2)
        cout<<mv.begin.x<<" "<<mv.begin.y<<"to"<<mv.end.x<<" "<<mv.end.y<<"and shoot"<<
        mv.obstacle.x<<" "<<mv.obstacle.y<<endl;
    } 
    cout<<move.size()<<endl;
    Move nxt_move=AI.get_move(bd);
    cout<<nxt_move.begin.x<<" "<<nxt_move.begin.y<<" to "<<nxt_move.end.x<<" "<<nxt_move.end.y<<" and shoot "<<
        nxt_move.obstacle.x<<" "<<nxt_move.obstacle.y<<endl;
    bd.make_move(nxt_move,-1);
    bd.print_board();
    /*cout<<bd.is_game_over(1)<<endl;
    bd.add({0,1},2);
    bd.add({0,3},2);
    bd.add({1,2},2);
    bd.add({2,1},2);
    bd.add({2,3},2);
    bd.add({3,2},2);
    bd.add({4,0},2);
    bd.add({5,1},2);
    bd.add({6,0},2);
    bd.add({7,1},2);
    bd.add({7,3},2);
    bd.add({6,2},2);
    bd.add({1,1},2);
    bd.add({1,3},2);
    bd.add({3,1},2);
    bd.add({4,1},2);
    bd.add({6,1},2);
    bd.add({6,3},2);
    bd.print_board();
    cout<<bd.is_game_over(1)<<endl;
    cout<<bd.is_game_over(-1)<<endl;
    bd.del({6,2});
    cout<<bd.is_game_over(1)<<endl;
    bd.print_board();*/
    //vector<vector<int>> grid=bd.get_grid();
    /*bd.add({1,1},1);
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
    bd.print_board();*/


}
/* int main(){
    //board_test();    
    return 0;
} */
/*   0   1   2   3   4   5   6   7
    +---+---+---+---+---+---+---+---+
  0 | . | . | a | . | . | a | . | . |
    +---+---+---+---+---+---+---+---+
  1 | . | . | . | . | . | . | . | . |
    +---+---+---+---+---+---+---+---+
  2 | a | . | . | . | . | . | . | a |
    +---+---+---+---+---+---+---+---+
  3 | . | . | . | . | . | . | . | . |
    +---+---+---+---+---+---+---+---+
  4 | . | . | . | . | . | . | . | . |
    +---+---+---+---+---+---+---+---+
  5 | b | . | . | . | . | . | . | b |
    +---+---+---+---+---+---+---+---+
  6 | . | . | . | . | . | . | . | . |
    +---+---+---+---+---+---+---+---+
  7 | . | . | b | . | . | b | . | . |
    +---+---+---+---+---+---+---+---+ */