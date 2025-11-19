#include "move.h"
Move::Move(){

}
Move::Move(const Position &initial_begin, const Position &initial_end,
const Position &initial_obstacle):
begin(initial_begin),end(initial_end),obstacle(initial_obstacle){
    
}
