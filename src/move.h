#pragma once
#include "position.h"
class Move{
    public:
        Position begin;
        Position end;
        Position obstacle;
        Move();
        void printmove()const;
        Move(const Position &initial_begin, const Position &initial_end, const Position &initial_obstacle);   
};