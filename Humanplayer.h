#pragma once
#include "Board.h"
class Humanplayer{
    public:
        Humanplayer(int color);
        Move get_move(const Board &now_board)const;
        ~Humanplayer();
    private:
        int player_color;
};