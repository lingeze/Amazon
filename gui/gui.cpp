#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include "BoardDrawer.h"
#include "PieceDrawer.h"
#include "Highlight.h"
#include "board.h"
#include <variant>
#include <iostream>
#include <algorithm>
const int row=8;
const int col=8;
const int board_size=8;
const float square_size=140;
enum class Gamestate{
    SELECTING_START,
    SELECTING_END,
    SELECTING_OBSTACLE
};
int main(){
    int current_color=1;
    BoardDrawer boarddrawer(board_size,square_size);
    PieceDrawer piecedrawer(square_size);
    unsigned int window_dimension=static_cast<unsigned int>(board_size*square_size);
    sf::RenderWindow window(sf::VideoMode({window_dimension,window_dimension}),L"亚马逊棋");
    window.setFramerateLimit(60);
    Board gameboard;
    gameboard.initialize();
    Gamestate current_state=Gamestate::SELECTING_START;
    Highlight HL(square_size);
    //gameboard.print_board();
    while (window.isOpen()){
        
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                if (mouseButtonPressed->button == sf::Mouse::Button::Left){
                    int select_row=static_cast<int>(mouseButtonPressed->position.y/square_size);
                    int select_col=static_cast<int>(mouseButtonPressed->position.x/square_size);
                    Position cur={select_row,select_col};
                    if(gameboard.is_in_board(cur)){
                        switch(current_state){
                            case Gamestate::SELECTING_START:
                                if(gameboard.is_valid_start(cur,current_color)){
                                    std::vector<Position> psb_move=gameboard.reach_positions(cur);
                                    HL.update(cur,psb_move);
                                    HL.set_move_color(1);
                                    current_state=Gamestate::SELECTING_END;
                                }
                                break;
                            case Gamestate::SELECTING_END:
                                std::cout<<HL.get_pos().x<<" "<<HL.get_pos().y<<" "<<cur.x<<" "<<cur.y<<std::endl;
                                if(gameboard.can_reach(HL.get_pos(),cur)){
                                    gameboard.del(HL.get_pos());
                                    gameboard.add(cur,current_color);
                                    HL.clear();
                                    HL.set_move_color(2);
                                    std::vector<Position> psb_move=gameboard.reach_positions(cur);
                                    HL.update(cur,psb_move);
                                    current_state=Gamestate::SELECTING_OBSTACLE;
                                }
                                break;
                            case Gamestate::SELECTING_OBSTACLE:  
                                std::cout<<HL.get_pos().x<<" "<<HL.get_pos().y<<" "<<cur.x<<" "<<cur.y<<std::endl;
                                if(gameboard.can_reach(HL.get_pos(),cur)){
                                    gameboard.add(cur,2);
                                    HL.clear();
                                    current_state=Gamestate::SELECTING_START;
                                    current_color=-current_color;
                                }
                                break;
                        }
                    }
                }
            }
        }
        window.clear(sf::Color(50,50,50));
        boarddrawer.draw(window);
        for (int i=0;i<row;i++){
            for (int j=0;j<col;j++){
                int color=gameboard.get_color(i,j);
                piecedrawer.draw(window,color,i,j);
            }
        }
        HL.draw(window);
        window.display();
    }
    return 0;
}