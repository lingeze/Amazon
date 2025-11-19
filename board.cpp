#include "board.h"
#include <iostream>
#include <cmath>
using std::vector;
Board::Board():row(8),col(8),grid(row,vector<int>(col,0)){

}
vector<vector<int>> Board::get_grid()const{
    return grid;
}
bool Board::check_move(const Move &move,int color)const{
    if(grid[move.begin.x][move.begin.y]!=color)return 0;
    if(!is_in_board(move.begin)||!is_in_board(move.end)||!is_in_board(move.obstacle))return 0;
    if(!can_reach(move.begin,move.end)||!can_reach(move.end,move.obstacle))return 0;
    return 1;
}
void Board::add(const Position &pos,const int &color){
    grid[pos.x][pos.y]=color;
}
void Board::del(const Position &pos){
    grid[pos.x][pos.y]=0;
}
void Board::print_board()const{
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(grid[i][j]==0)std::cout<<".";
            else if(grid[i][j]==1)std::cout<<"a";
            else if(grid[i][j]==-1)std::cout<<"b";
            else if(grid[i][j]==2)std::cout<<"#";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}
bool Board::is_in_board(const Position &pos)const{
    if(pos.x<0||pos.x>=row||pos.y<0||pos.y>=col)return 0;
    return 1;
}
bool Board::can_reach(const Position &begin,const Position &end)const{
    if(begin==end)return 0;
    if(grid[end.x][end.y])return 0;
    int bgx=std::min(begin.x,end.x);
    int bgy=std::min(begin.y,end.y);
    int edx=std::max(begin.x,end.x);
    int edy=std::max(begin.y,end.y);
    if((edy-bgy)==(edx-bgx)){
        int addx=((bgx==begin.x)?1:-1);
        int addy=((bgy==begin.y)?1:-1);
        for(int i=begin.x+addx,j=begin.y+addy;i!=edx&&j!=edy;i+=addx,j+=addy){
            if(grid[i][j])return 0;
        }
    }
    else if(bgx==edx){
        for(int i=bgy+1;i<=edy;i++){
            if(grid[bgx][i])return 0;
        }
    }
    else if(bgy==edy){
        for(int i=bgx+1;i<=edx;i++){
            if(grid[i][bgy])return 0;
        }
    }
    else return 0;
    return 1;
}