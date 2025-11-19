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
    bool check_move(const Move &move,int color)const;
    void add(const Position &pos,const int &color);
    void del(const Position &pos);
    void print_board()const;
    private:
    int row,col;
    vector<vector<int>> grid;
};