#pragma once
#include <vector>
#include "move.h"
using std::vector;
class Board{
    public:
    Board();
    vector<vector<int>> get_grid()const;
    bool can_reach(const Position &begin,const Position &end)const;
    bool is_in_board(const Position &pos)const;
    bool check_move(const Move &move,const int &color)const;
    void print_board()const;
    void make_move(const Move &move,const int &color);
    bool is_game_over(const int &next_color)const;
    void initialize();
    
    private:
    int row,col;
    vector<vector<int>> grid;
    char transform(int i,int j)const;
    void add(const Position &pos,const int &color);
    void del(const Position &pos);
    bool can_move(const Position &pos)const;
};