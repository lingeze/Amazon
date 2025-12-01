#pragma once
#include "board.h"
class filemanage{
    static void list_saves();
    static void savefile(const Board &gameboard,const int &current_player);
    static void loadfile(Board &gameboard,int &current_player);
};