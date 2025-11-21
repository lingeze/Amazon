#pragma once
#include "player.h"
class Humanplayer:public Player{
    public:
        Move get_move(const Board &now_board)const;
        ~Humanplayer();
};