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
    void undo_move(const Move &move,const int &color);
    bool is_game_over(const int &next_color)const;
    void add(const Position &pos,const int &color);
    void del(const Position &pos);
    void initialize();
    vector<Position> get_start_position(int color)const;
    vector<Move> get_all_psb_move(int color)const;
    double calc_board_score(const int &next_color)const;
    //private:
    int row,col;
    vector<vector<int>> N;
    void N_update(const Position &pos);
    void N_calculate(const Position &pos);
    vector<double> calc_alpha(const int &color)const;
    double calc_f(double w,double alpha)const;
    double calc_m()const;
    vector<double> pow_2;
    mutable vector<vector<int>> grid;
    char transform(int i,int j)const;
    bool can_move(const Position &pos)const;
    double calc_pow(const int &num)const{
        if(num>pow_2.size())return 0; 
        return pow_2[num];
    }
    void pow_initialize(){
        pow_2.resize(40,0);pow_2[0]=1;
        for(int i=1;i<39;i++){
            pow_2[i]=pow_2[i-1]/2;
        }
    }    
    vector<vector<int>> queen_dist_grid(const Position &pos)const;
    vector<vector<int>> king_dist_grid(const Position &pos)const;
    vector<vector<int>> min_queen_dist_grid(const int &color)const;
    vector<vector<int>> min_king_dist_grid(const int &color)const;
    double calc_w(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const;
    double calc_t(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2,const int &color)const;
    double calc_c1(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const;
    double calc_c2(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const;
};