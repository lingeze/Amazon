#pragma once
#include <random>
#include "Board.h"
class AIplayer{
    public:
        AIplayer(int Color);
        Move get_move(const Board &now_board)const;
        ~AIplayer();
    private:
        int player_color;
        mutable std::mt19937 gen;
        Move rand_strategy(const Board &now_board)const;
};