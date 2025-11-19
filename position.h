#pragma once
class Position{
    public:
        int x;int y;
        Position();
        Position(int initial_x,int initial_y);
        bool operator==(const Position &other)const;
};
