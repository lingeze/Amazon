#pragma once
#include <random>
#include <chrono>
#include "Board.h"
class AIplayer{
    public:
        AIplayer(int Color);
        Move get_move(Board now_board,int color);
        void start_time_reset(){
            start_time=std::chrono::steady_clock::now();
        }
    private:
        int search_cnt=0;
        std::chrono::steady_clock::time_point start_time;
        bool stop_search=0;
        int player_color;
        mutable std::mt19937 gen;
        Move rand_strategy(const Board &now_board)const;
        double alpha_beta_search(Board &now_board,const int &color,const int &dep,double alpha,double beta);
        Move minmax_strategy(const Board &gameboard,int color);
        bool is_reach_time_limit();
};