#pragma once
#include "Board.h"
#include <string>
class Humanplayer{
    public:
        Humanplayer(int color);
        Move get_move(std::string user_input,const Board &now_board)const;
        ~Humanplayer();
    private:
        int player_color;
        Position to_position(std::string x,std::string y)const;
};