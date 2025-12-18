#pragma once
#include "board.h"
class Filemanage{
    public:
    static bool list_saves();
    static bool savefile(const Board &gameboard,const int &human_color,const int &current_player,const int &savenumber);
    static bool loadfile(Board &gameboard,int &human_color,int &current_player,const int &savenumber);
};