#pragma once
#include "board.h"
class Filemanage{
    public:
    static bool list_saves();
    static bool savefile(const Board &gameboard,const int &current_player,const int &savenumber);
    static bool loadfile(Board &gameboard,int &current_player,const int &savenumber);
};