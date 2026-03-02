#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include "BoardDrawer.h"
#include "PieceDrawer.h"
#include "Highlight.h"
#include "button.h"
#include "board.h"
#include "filemanage.h"
#include "AIplayer.h"
#include <variant>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
const int row=8;
const int col=8;
const int board_size=8;
const float square_size=140;
enum class GameState{
    MAIN_MENU,   
    CHOOSE_SIDE,  
    PLAYING,  
    PAUSE,
    SAVE,
    LOAD,
    CONFIRM_OVERWRITE,
    LOAD_ERROR,
    GAME_OVER 
};
enum class Turnstate{
    SELECTING_START,
    SELECTING_END,
    SELECTING_OBSTACLE
};
int main(){
    int current_color=1;
    BoardDrawer boarddrawer(board_size,square_size);
    PieceDrawer piecedrawer(square_size);
    unsigned int window_dimension=static_cast<unsigned int>(board_size*square_size);
    float function_width=400.f;
    
    sf::Font font;
    if (!font.openFromFile("assets/simhei.ttf")) {
        std::cerr << "Failed to load font file" << std::endl;
        return 1;
    }
    Board gameboard;Filemanage filemanager;

    Button startbutton(font,L"开始游戏",24);
    Button endbutton(font,L"结束游戏",24);
    Button continuebutton(font,L"继续游戏",24);
    Button whitebutton(font,L"执白方",24);
    Button blackbutton(font,L"执黑方",24);
    Button pausebutton(font,L"暂停",24);
    Button savebutton(font,L"存档",24);
    Button loadbutton(font,L"读档",24);
    Button returnbutton(font,L"结束",24);
    Button releasebutton(font,L"继续",24);
    float buttonWidth1=600.f;
    float buttonHeight1=200.f;
    float buttonWidth2=300.f;
    float buttonHeight2=100.f;
    float centerX=(window_dimension+function_width)/2.f-buttonWidth1/2.f;
    float centerY=window_dimension/2.f-buttonHeight1/2.f;
    float function_button_x=window_dimension+(function_width-buttonWidth2)/2.f;
    float function_button_y=centerY;
    startbutton.setupButton({centerX,centerY-250},{buttonWidth1,buttonHeight1});
    whitebutton.setupButton({centerX,centerY+200},{buttonWidth1,buttonHeight1});
    blackbutton.setupButton({centerX,centerY-200},{buttonWidth1,buttonHeight1});
    endbutton.setupButton({centerX,centerY+250},{buttonWidth1,buttonHeight1});
    continuebutton.setupButton({centerX,centerY},{buttonWidth1,buttonHeight1});
    pausebutton.setupButton({function_button_x,function_button_y-150-75},{buttonWidth2,buttonHeight2});
    savebutton.setupButton({function_button_x,function_button_y-75},{buttonWidth2,buttonHeight2});
    loadbutton.setupButton({function_button_x,function_button_y+75},{buttonWidth2,buttonHeight2});
    returnbutton.setupButton({function_button_x,function_button_y+150+75},{buttonWidth2,buttonHeight2});
    releasebutton.setupButton({function_button_x,function_button_y-150-75},{buttonWidth2,buttonHeight2});

    GameState last_gamestate=GameState::MAIN_MENU;
    std::vector<Button> slotbuttons;
    Button backbutton(font,L"返回",24);
    float slotWidth = 600.f;
    float slotHeight = 120.f;
    float startX=((window_dimension + function_width) / 2.f) - (slotWidth * 2 + 50) / 2;
    float startY=200.f;
    float gap=20.f;
    for(int i=0;i<10;i++){
        std:: wstring slotText = L"档位 " + std::to_wstring(i);
        if(i==0)slotText = L"自动存档";
        if(filemanager.file_exists(i)){
            slotText += L"(已使用)";
        }
        else {
            slotText += L"(空)";
        }
        Button slotbutton(font,slotText,24);
        slotbutton.setupbuttoncolor(sf::Color(100, 200, 100));
        float x = startX+(i % 2)*(slotWidth + 50);
        float y = startY + (i / 2) * (slotHeight + gap);
        slotbutton.setupButton({x, y}, {slotWidth, slotHeight});
        slotbuttons.push_back(slotbutton); 
    }
    backbutton.setupButton({((window_dimension + function_width) / 2.f) 
        - 150.f, startY + 5 * (slotHeight + gap) + 30.f}, {300.f, 80.f});
    
    int selected_slot=-1;
    sf::RectangleShape popupBackground;
    sf::Text popupText(font, "", 80);
    Button yesbutton(font,L"是",24);
    Button nobutton(font,L"否",24);
    Button okbutton(font,L"确定",24);
    popupBackground.setSize({(window_dimension+function_width)/1.5f,window_dimension/1.5f});
    popupBackground.setFillColor(sf::Color(200, 200, 200));
    popupBackground.setOutlineColor(sf::Color::White);
    popupBackground.setOutlineThickness(2.f);
    popupBackground.setOrigin(popupBackground.getLocalBounds().getCenter());
    popupBackground.setPosition({(window_dimension+function_width)/2.f,window_dimension/2.f});
    sf::Vector2f popupPosition = popupBackground.getPosition();
    yesbutton.setupButton({popupPosition.x-150-200/2, popupPosition.y+100}, {200.f, 100.f});
    nobutton.setupButton({popupPosition.x+150-200/2, popupPosition.y+100}, {200.f, 100.f});
    okbutton.setupButton({popupPosition.x-200/2, popupPosition.y + 100}, {200, 100});


    gameboard.initialize();
    GameState current_gamestate=GameState::MAIN_MENU;
    Turnstate current_state=Turnstate::SELECTING_START;
    Highlight HL(square_size);
    int human_color=1;
    AIplayer* ai=nullptr;
    Move ai_move;
    sf::Clock animation_clock; 
    //gameboard.print_board();
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(window_dimension+function_width),window_dimension}),L"亚马逊棋");
    window.setFramerateLimit(60);
    while (window.isOpen()){
        switch(current_gamestate){
            case GameState::MAIN_MENU:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                            sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                            if (startbutton.isMouseOver(mousePos)) {
                                gameboard.clear();
                                gameboard.initialize();
                                HL.clear();
                                current_color=1;
                                current_gamestate = GameState::CHOOSE_SIDE;
                            }
                            else if(endbutton.isMouseOver(mousePos)){
                                window.close();
                            }
                            else if(continuebutton.isMouseOver(mousePos)){
                                last_gamestate=current_gamestate;
                                current_gamestate=GameState::LOAD;
                            }
                        }
                    }
                }
                window.clear(sf::Color(200, 200, 200));
                startbutton.draw(window);
                endbutton.draw(window);
                continuebutton.draw(window);   
                window.display();
                break;
            }
            case GameState::CHOOSE_SIDE:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                            sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                            bool side_chosen = false;
                            if (blackbutton.isMouseOver(mousePos)) {
                                human_color = 1;
                                side_chosen = true;
                            }
                            else if (whitebutton.isMouseOver(mousePos)) {
                                human_color = -1;
                                side_chosen = true;
                            }
                            if (side_chosen) {
                                delete ai;
                                ai = new AIplayer(-human_color);
                                current_state = Turnstate::SELECTING_START;
                                current_gamestate = GameState::PLAYING;
                            }
                        }
                    }
                }
                window.clear(sf::Color(200, 200, 200));
                blackbutton.draw(window);
                whitebutton.draw(window);
                window.display();
                break;
                current_gamestate=GameState::PLAYING;
                break;
            }
            case GameState::PLAYING:{
                if(gameboard.is_game_over(current_color)){
                    std::cout<<"Game Over Detected"<<std::endl;
                    current_gamestate=GameState::GAME_OVER;
                    break;
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
                pausebutton.draw(window);
                savebutton.draw(window);
                loadbutton.draw(window);
                returnbutton.draw(window);
                window.display();
                //AI move
                if(current_color==-human_color){
                    while (auto event = window.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                            window.close();
                        }
                    }
                    switch (current_state){
                        case Turnstate::SELECTING_START:{
                            ai->start_time_reset();
                            ai_move=ai->get_move(gameboard,-human_color);
                            std::vector<Position> psb_move=gameboard.reach_positions(ai_move.begin);
                            HL.update(ai_move.begin,psb_move);
                            HL.set_move_color(1);
                            current_state=Turnstate::SELECTING_END;
                            animation_clock.restart();
                            break;
                        }
                        case Turnstate::SELECTING_END:{
                            if(animation_clock.getElapsedTime().asMilliseconds()<500)break;
                            gameboard.del(HL.get_pos());
                            gameboard.add(ai_move.end,current_color);
                            HL.clear();
                            HL.set_move_color(2);
                            std::vector<Position> psb_move=gameboard.reach_positions(ai_move.end);
                            HL.update(ai_move.end,psb_move);
                            current_state=Turnstate::SELECTING_OBSTACLE;
                            animation_clock.restart();
                            break;
                        }
                        case Turnstate::SELECTING_OBSTACLE:{
                            if(animation_clock.getElapsedTime().asMilliseconds()<700)break;
                            gameboard.add(ai_move.obstacle,2);
                            HL.clear();
                            filemanager.savefile(gameboard,human_color,current_color,0);
                            current_state=Turnstate::SELECTING_START;
                            current_color=-current_color;
                            break;
                        }
                    }
                }
                else {
                    //human move
                    while (auto event = window.pollEvent()) {
                        if (event->is<sf::Event::Closed>()) {
                            window.close();
                        }    
                        if(current_color!=human_color)continue;
                        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                                sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                                if(pausebutton.isMouseOver(mousePos)){
                                    current_gamestate=GameState::PAUSE;
                                    break;
                                }
                                else if(savebutton.isMouseOver(mousePos)){
                                    last_gamestate=current_gamestate;
                                    current_gamestate=GameState::SAVE;
                                    break;
                                    //gameboard.save_game("savefile.txt",current_color,current_state);
                                }
                                else if(loadbutton.isMouseOver(mousePos)){
                                    last_gamestate=current_gamestate;
                                    current_gamestate=GameState::LOAD;
                                    break;
                                    //gameboard.load_game("savefile.txt",current_color,current_state);
                                    //HL.clear();
                                }
                                else if(returnbutton.isMouseOver(mousePos)){
                                    current_gamestate=GameState::MAIN_MENU;
                                    break;
                                    //gameboard.initialize();
                                    //HL.clear();
                                    //current_color=1;
                                }
                            }
                        }
                        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                            if (mouseButtonPressed->button == sf::Mouse::Button::Left){
                                sf::Vector2f mousePos = 
                                    window.mapPixelToCoords({ mouseButtonPressed->position.x, 
                                    mouseButtonPressed->position.y });
                                int select_row=static_cast<int>(mousePos.y/square_size);
                                int select_col=static_cast<int>(mousePos.x/square_size);
                                Position cur={select_row,select_col};
                                if(gameboard.is_in_board(cur)){
                                    switch(current_state){
                                        case Turnstate::SELECTING_START:
                                            if(gameboard.is_valid_start(cur,current_color)){
                                                std::vector<Position> psb_move=gameboard.reach_positions(cur);
                                                HL.update(cur,psb_move);
                                                HL.set_move_color(1);
                                                current_state=Turnstate::SELECTING_END;
                                            }
                                            break;
                                        case Turnstate::SELECTING_END:
                                            //std::cout<<HL.get_pos().x<<" "<<HL.get_pos().y<<" "<<cur.x<<" "<<cur.y<<std::endl;
                                            if(gameboard.can_reach(HL.get_pos(),cur)){
                                                gameboard.del(HL.get_pos());
                                                gameboard.add(cur,current_color);
                                                HL.clear();
                                                HL.set_move_color(2);
                                                std::vector<Position> psb_move=gameboard.reach_positions(cur);
                                                HL.update(cur,psb_move);
                                                current_state=Turnstate::SELECTING_OBSTACLE;
                                            }
                                            break;
                                        case Turnstate::SELECTING_OBSTACLE:  
                                            //std::cout<<HL.get_pos().x<<" "<<HL.get_pos().y<<" "<<cur.x<<" "<<cur.y<<std::endl;
                                            if(gameboard.can_reach(HL.get_pos(),cur)){
                                                gameboard.add(cur,2);
                                                HL.clear();
                                                current_state=Turnstate::SELECTING_START;
                                                current_color=-current_color;
                                            }
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }
            case GameState::PAUSE:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                            sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                            if(releasebutton.isMouseOver(mousePos)){
                                current_gamestate=GameState::PLAYING;
                                break;
                            }
                            else if (savebutton.isMouseOver(mousePos)) {
                                last_gamestate=current_gamestate;
                                current_gamestate=GameState::SAVE;
                                break;
                                //gameboard.save_game("savefile.txt",current_color,current_state);
                            }
                            else if (loadbutton.isMouseOver(mousePos)) {
                                last_gamestate=current_gamestate;
                                current_gamestate=GameState::LOAD;
                                break;
                                //gameboard.load_game("savefile.txt",current_color,current_state);
                                //HL.clear();
                            }
                            else if (returnbutton.isMouseOver(mousePos)) {
                                current_gamestate = GameState::MAIN_MENU;
                                break;
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
                sf::RectangleShape overlay;
                overlay.setSize({static_cast<float>(window_dimension+function_width),static_cast<float>(window_dimension)});
                overlay.setFillColor(sf::Color(200, 200, 200, 150));
                window.draw(overlay);
                std::wstring resultstring=L"已暂停";
                sf::Text pauseText(font, resultstring, 200);
                pauseText.setFillColor(sf::Color::Black);
                pauseText.setStyle(sf::Text::Bold);
                sf::FloatRect textRect = pauseText.getLocalBounds();
                pauseText.setOrigin(textRect.getCenter());
                pauseText.setPosition({(window_dimension)/2.f, window_dimension/2.f});
                window.draw(pauseText);
                releasebutton.draw(window);
                savebutton.draw(window);
                loadbutton.draw(window);
                returnbutton.draw(window);
                window.display();
                break;
            }
            case GameState::SAVE:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                        sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                        if(backbutton.isMouseOver(mousePos)){
                            current_gamestate=last_gamestate;
                            break;
                        }
                        for(int i=0;i<slotbuttons.size();i++){
                            if(slotbuttons[i].isMouseOver(mousePos)){
                                if(filemanager.file_exists(i)){
                                    //confirm overwrite
                                    selected_slot=i;
                                    current_gamestate=GameState::CONFIRM_OVERWRITE;
                                    break;
                                }   
                                if(filemanager.savefile(gameboard,human_color,current_color,i)){
                                    std::wstring slotText;
                                    if(i==0)slotText = L"自动存档(已使用)";
                                    else slotText = L"档位 " + std::to_wstring(i)+ L"(已使用)";
                                    slotbuttons[i].setuptext(slotText);
                                }
                                else{
                                    std::cout<<"Save Failed!"<<std::endl;
                                }
                                break;
                            }
                        }
                    }            
                }
                sf::RectangleShape overlay;
                overlay.setSize({static_cast<float>(window_dimension+function_width),static_cast<float>(window_dimension)});
                overlay.setFillColor(sf::Color(255,255,255,150));
                window.draw(overlay);
                for(int i=0;i<slotbuttons.size();i++){
                    slotbuttons[i].draw(window);
                }
                backbutton.draw(window);
                window.display();
    
                break;
            }
            case GameState::CONFIRM_OVERWRITE:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                        sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                        if(yesbutton.isMouseOver(mousePos)){
                            if(filemanager.savefile(gameboard,human_color,current_color,selected_slot)){
                                std::wstring slotText;
                                if(selected_slot==0)slotText = L"自动存档(已使用)";
                                else slotText = L"档位 " + std::to_wstring(selected_slot)+ L"(已使用)";
                                slotbuttons[selected_slot].setuptext(slotText);
                            }
                            else{
                                std::cout<<"Save Failed!"<<std::endl;
                            }
                            current_gamestate=GameState::SAVE;
                            break;
                        }
                        else if(nobutton.isMouseOver(mousePos)){
                            current_gamestate=GameState::SAVE;
                            break;
                        }
                    }            
                }
                sf::RectangleShape overlay;
                overlay.setSize({static_cast<float>(window_dimension+function_width),static_cast<float>(window_dimension)});
                overlay.setFillColor(sf::Color(255,255,255,150));
                window.draw(overlay);
                window.draw(popupBackground);
                popupText.setString(L"该档位已被使用，是否覆盖？");
                sf::FloatRect textRect = popupText.getLocalBounds();
                popupText.setOrigin(textRect.getCenter());
                popupText.setFillColor(sf::Color::Black);
                popupText.setPosition({popupPosition.x,popupPosition.y-50});
                window.draw(popupText);
                yesbutton.draw(window);
                nobutton.draw(window);
                window.display();
                break;
            }   
            case GameState::LOAD:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                        sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                        if(backbutton.isMouseOver(mousePos)){
                            current_gamestate=last_gamestate;
                            break;
                        }
                        for(int i=0;i<slotbuttons.size();i++){
                            if(slotbuttons[i].isMouseOver(mousePos)){
                                if(filemanager.file_exists(i)){
                                    if(filemanager.loadfile(gameboard,human_color,current_color,i)){
                                        HL.clear();
                                        current_state=Turnstate::SELECTING_START;
                                        delete ai;
                                        ai = new AIplayer(-human_color);
                                        current_gamestate=GameState::PLAYING;
                                    }
                                    else{
                                        std::cout<<"Load Failed!"<<std::endl;
                                    }
                                }
                                else {
                                    selected_slot=i;
                                    current_gamestate=GameState::LOAD_ERROR;
                                }
                            }
                        }
                    }            
                }
                sf::RectangleShape overlay;
                overlay.setSize({static_cast<float>(window_dimension+function_width),static_cast<float>(window_dimension)});
                overlay.setFillColor(sf::Color(255,255,255,150));
                window.draw(overlay);
                for(int i=0;i<slotbuttons.size();i++){
                    slotbuttons[i].draw(window);
                }
                backbutton.draw(window);
                window.display();
                break;
            }
            case GameState::LOAD_ERROR:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
                        sf::Vector2f mousePos = window.mapPixelToCoords({ mouseButtonPressed->position.x, mouseButtonPressed->position.y });
                        if(okbutton.isMouseOver(mousePos)){
                            current_gamestate=GameState::LOAD;
                            break;
                        }
                    }            
                    }
                sf::RectangleShape overlay;
                overlay.setSize({static_cast<float>(window_dimension+function_width),static_cast<float>(window_dimension)});
                overlay.setFillColor(sf::Color(255,255,255,150));
                window.draw(overlay);
                window.draw(popupBackground);
                popupText.setString(L"该档位无存档文件！");
                popupText.setFillColor(sf::Color::Black);
                sf::FloatRect textRect = popupText.getLocalBounds();
                popupText.setOrigin(textRect.getCenter());
                popupText.setPosition({popupPosition.x,popupPosition.y-50});
                window.draw(popupText);
                okbutton.draw(window);
                window.display();
                break;
            }
            case GameState::GAME_OVER:{
                while (const auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                        current_gamestate = GameState::MAIN_MENU;
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
                sf::RectangleShape overlay;
                overlay.setSize({static_cast<float>(window_dimension+function_width),static_cast<float>(window_dimension)});
                overlay.setFillColor(sf::Color(255,255,255,150));
                window.draw(overlay);
                std::wstring resultstring;
                if(current_color==human_color){
                    resultstring=L"您输了！";
                }
                else{
                    resultstring=L"您赢了！";
                }
                sf::Text gameOverText(font, resultstring, 200);
                gameOverText.setFillColor(sf::Color::Black);
                gameOverText.setStyle(sf::Text::Bold);
                sf::FloatRect textRect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(textRect.getCenter());
                gameOverText.setPosition({(window_dimension)/2.f, window_dimension/2.f-100.f});
                window.draw(gameOverText);
                sf::Text promptText(font, L"点击任意位置返回主菜单", 60);
                promptText.setFillColor(sf::Color::Black);
                sf::FloatRect promptRect = promptText.getLocalBounds();
                promptText.setOrigin(promptRect.getCenter());
                promptText.setPosition({(window_dimension)/2.f, window_dimension/2.f + 150.f});
                window.draw(promptText);
                pausebutton.draw(window);
                savebutton.draw(window);
                loadbutton.draw(window);
                returnbutton.draw(window);
                window.display();
                break;
            }
        }
    }
    return 0;
}