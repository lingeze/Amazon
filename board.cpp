#include "board.h"
#include <iostream>
#include <cmath>
using std::vector;
Board::Board():row(8),col(8),grid(row,vector<int>(col,0)){

}
vector<vector<int>> Board::get_grid()const{
    return grid;
}
int Board::get_color(const int &pos_x,const int &pos_y)const{
    return grid[pos_x][pos_y];
}
bool Board::check_move(const Move &move,const int &color)const{
    if(!is_in_board(move.begin)||!is_in_board(move.end)||!is_in_board(move.obstacle))return 0;
    if(grid[move.begin.x][move.begin.y]!=color)return 0;
    //if(!)return 0;
    grid[move.begin.x][move.begin.y]=0;
    bool ok=can_reach(move.begin,move.end)&&can_reach(move.end,move.obstacle);
    grid[move.begin.x][move.begin.y]=color;
    if(!ok)return 0;
    return 1;
}
void Board::add(const Position &pos,const int &color){
    grid[pos.x][pos.y]=color;
}
void Board::del(const Position &pos){
    grid[pos.x][pos.y]=0;
}
char Board::transform(int i,int j)const{
    if(grid[i][j]==0)return '.';
    else if(grid[i][j]==1)return 'a';
    else if(grid[i][j]==-1)return 'b';
    else if(grid[i][j]==2)return '#';
    return ' ';
}
void Board::print_board()const{
    std::cout<<"    ";
    for(int i=0;i<col;i++){
        std::cout<<"  "<<i<<" ";
    }
    std::cout<<std::endl;
    for(int i=0;i<row;i++){
        std::cout<<"    ";
        std::cout<<"+---+---+---+---+---+---+---+---+"<<std::endl;
        std::cout<<"  "<<i<<" ";
        for(int j=0;j<col;j++){
            std::cout<<"|";
            std::cout<<" ";
            std::cout<<transform(i,j);
            std::cout<<" ";
        }
        std::cout<<"|";
        std::cout<<std::endl;
    }
    std::cout<<"    +---+---+---+---+---+---+---+---+"<<std::endl;
    std::cout<<std::endl;
}
bool Board::is_in_board(const Position &pos)const{
    if(pos.x<0||pos.x>=row||pos.y<0||pos.y>=col)return 0;
    return 1;
}
bool Board::can_reach(const Position &begin,const Position &end)const{
    if(begin==end)return 0;
    if(grid[end.x][end.y])return 0;
    int minx=std::min(begin.x,end.x);
    int miny=std::min(begin.y,end.y);
    int maxx=std::max(begin.x,end.x);
    int maxy=std::max(begin.y,end.y);
    if((maxy-miny)==(maxx-minx)){
        int addx=((minx==begin.x)?1:-1);
        int addy=((miny==begin.y)?1:-1);
        for(int i=begin.x+addx,j=begin.y+addy;i!=end.x&&j!=end.y;i+=addx,j+=addy){
            if(grid[i][j])return 0;
        }
    }
    else if(minx==maxx){
        for(int i=miny;i<=maxy;i++){
            //std::cout<<bgx<<" "<<i<<endl;
            if(grid[minx][i]){/*std::cout<<bgx<<" "<<i<<std::endl;*/return 0;}
        }
    }
    else if(miny==maxy){
        for(int i=minx;i<=maxx;i++){
            if(grid[i][miny])return 0;
        }
    }
    else return 0;
    return 1;
}
void Board::make_move(const Move &move,const int &color){
    del(move.begin);
    add(move.end,color);
    add(move.obstacle,2);
}

bool Board::is_game_over(const int &next_color)const{
    //vector<Position> chess_pieces;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(grid[i][j]==next_color)if(can_move({i,j}))return 0;
        }
    }
    return 1;
}
bool Board::can_move(const Position &pos)const{
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    for(int id=0;id<=7;id++){
        int nx=pos.x+way_x[id],ny=pos.y+way_y[id];
        if(!is_in_board({nx,ny}))continue;
        if(!grid[nx][ny])return 1;
        
    }
    return 0;
}
void Board::initialize(){
    add({0,2},1);
    add({2,0},1);
    add({0,5},1);
    add({2,7},1);
    add({5,0},-1);
    add({7,2},-1);
    add({5,7},-1);
    add({7,5},-1);
}
vector<Position> Board::get_start_position(int color)const{
    vector<Position> start;
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++){
            if(grid[i][j]==color)start.push_back({i,j});
        }
    }
    return start;
}
vector<Move> Board::get_all_psb_move(int color)const{
    vector<Position> start=get_start_position(color);
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    vector<Position> end;
    vector<Move> psb_move;
    for(const auto &st:start){
        end.clear();
        for(int way_id=0;way_id<=7;way_id++){
            for(int nx=st.x+way_x[way_id],ny=st.y+way_y[way_id];
                nx<=7&&ny<=7&&nx>=0&&ny>=0;
                nx+=way_x[way_id],ny+=way_y[way_id]
            ){
                if(grid[nx][ny])break;
                end.push_back({nx,ny});
            }
        }
        for(const auto &ed:end){
            for(int way_id=0;way_id<=7;way_id++){
                for(int nx=ed.x+way_x[way_id],ny=ed.y+way_y[way_id];
                    nx<=7&&ny<=7&&nx>=0&&ny>=0;
                    nx+=way_x[way_id],ny+=way_y[way_id]
                ){
                    if(grid[nx][ny]&&grid[nx][ny]!=color)break;
                    if(grid[nx][ny]==color&&(nx!=st.x||ny!=st.y))break;
                    psb_move.push_back({st,ed,{nx,ny}});
                }
            }
        } 
        //std::cout<<end.size()<<std::endl;
    }
    return psb_move;
}