#include "AIPlayer.h"
#include "Humanplayer.h"
#include <iostream>
#include <string>
void Gameloop(){
    Board gameboard; 
    int current_player=1;
    gameboard.initialize();
    //选择界面
    std::string input;
    std::cout<<"请选择相应操作"<<std::endl;
    std::cout<<"A:开始游戏"<<std::endl;
    std::cout<<"B:继续游戏"<<std::endl;
    std::cout<<"C:退出游戏"<<std::endl;
    std::cin>>input;
    while(input!="A"&&input!="B"&&input!="C"){
        std::cout<<"您的输入有误，请重新输入"<<std::endl;
        std::cout<<"请选择相应操作"<<std::endl;
        std::cout<<"A:开始游戏"<<std::endl;
        std::cout<<"B:继续游戏"<<std::endl;
        std::cout<<"C:退出游戏"<<std::endl;
        std::cin>>input;
    }
    if(input=="A"){
        std::string select_color;
        std::cout<<"请选择您希望的落子顺序"<<std::endl;
        std::cout<<"A: 您先手"<<std::endl;
        std::cout<<"B: AI先手"<<std::endl;
        std::cin>>select_color;
        while(select_color!="A"&&select_color!="B"){
            std::cout<<"您的输入有误，请重新输入"<<std::endl;
            std::cout<<"请选择您希望的落子顺序"<<std::endl;
            std::cout<<"A: 您先手"<<std::endl;
            std::cout<<"B: AI先手"<<std::endl;
        }
        if(select_color=="A")current_player=1;
        else current_player=-1;
    }
    if(input=="B"){

    }
    if(input=="C"){
        return;
    }
    //游戏进行
    AIplayer ai(-1);
    Humanplayer human(1);
    while(1){
        gameboard.print_board();
        
        human.get_move();
    }
}
/*
int main(){
    Gameloop();
    return 0;
}*/