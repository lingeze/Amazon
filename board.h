#pragma once
#include <vector>
#include "move.h"
using std::vector;
class Board{
    public:
    Board();
    vector<vector<int>> get_grid()const;
    int get_color(const int &pos_x,const int &pos_y)const;
    bool can_reach(const Position &begin,const Position &end)const;
    bool is_in_board(const Position &pos)const;
    bool check_move(const Move &move,const int &color)const;
    void print_board()const;
    void make_move(const Move &move,const int &color);
    bool is_game_over(const int &next_color)const;
    void add(const Position &pos,const int &color);
    void del(const Position &pos);
    void initialize();
    vector<Position> get_start_position(int color)const;
    vector<Move> get_all_psb_move(int color)const;
    private:
    int row,col;
    mutable vector<vector<int>> grid;
    char transform(int i,int j)const;
    bool can_move(const Position &pos)const;
};