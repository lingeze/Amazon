#pragma once 
#include "board.h"
class Player{
    public:
    int color;
    virtual Move get_move(const Board &now_board)const = 0;
    virtual ~Player(){}
};