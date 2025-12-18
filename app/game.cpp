#include "AIPlayer.h"
#include "Humanplayer.h"
#include "filemanage.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h> // 必须包含这个头文件来使用 Windows API

/**
 * 在 Windows 终端上启用 ANSI 转义码处理功能。
 * 这是让 clear_screen() 生效的关键。
 * 如果成功启用，返回 true；否则返回 false。
 */
bool enable_virtual_terminal_processing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return false;
    }

    // 添加 ENABLE_VIRTUAL_TERMINAL_PROCESSING 标志
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return false;
    }

    return true;
}
void clear_screen() {
    // \033[2J: 清除整个屏幕
    // \033[H:  将光标移动到左上角 (Home)
    enable_virtual_terminal_processing();
    std::cout << "\033[2J\033[H"<<std::flush;
}
void play_a_game(Board gameboard,int human_color,int current_player,bool is_continue){
    //游戏进行
    AIplayer ai(-human_color);
    Humanplayer human(human_color);
    std::cout<<"若想进行存盘、读盘、暂停、结束等操作，请您在输入坐标前输入相应的指令："<<std::endl;
    std::cout<<"存盘: s"<<std::endl;
    std::cout<<"读盘: l"<<std::endl;
    std::cout<<"暂停: p"<<std::endl;
    std::cout<<"结束: e"<<std::endl;
    std::cout<<"输入回车键开始游戏"<<std::endl;
    std::cin.get();
    std::cout<<"游戏开始！"<<std::endl;
    while(1){
        Move now_move;
        
        gameboard.print_board(); 
        //std::cout<<"testsdfsdfsadfds"<<std::endl;
        //判断是否结束
        if(gameboard.is_game_over(current_player)==1){
            if(current_player==human_color){
                std::cout<<"您输了！"<<std::endl;
            }
            else std::cout<<"您赢了！！！"<<std::endl;
            return;
        }
        Filemanage::savefile(gameboard,human_color,current_player,0);
        bool input_l=0;
        //处理用户指令
        if(current_player==human_color){
            bool is_input_valid=0;
            while(!is_input_valid){
                std::string user_input;
                std::cout<<"轮到您了。"<<std::endl;
                std::cout<<"若想下一步棋，请依次输入x1 y1 x2 y2 x3 y3，以空格分隔，分别代表选择棋子的起始位置、落子位置和释放障碍物的位置，注意先列后行。"<<std::endl;
                std::cout<<"若想输入指令，请输入单个字母：s（存盘）、l（读盘）、p（暂停）、e（结束）"<<std::endl;
                std::getline(std::cin,user_input);
                //存盘
                if(user_input=="s"){
                    Filemanage::list_saves();
                    std::string input;
                    std::cout<<"请输入您要保存的存档位置(1~9)：";
                    std::getline(std::cin,input);
                    while(input.size()>1||input[0]<'1'||input[0]>'9'){
                        std::cout<<"您输入的格式有误。请重新输入"<<std::endl;
                        std::cout<<"请输入您要保存的存档位置(1~9)：";
                        std::getline(std::cin,input);
                    }
                    int id=input[0]-'0';
                    while(!Filemanage::savefile(gameboard,human_color,current_player,id)){
                        std::cout<<"请重新输入"<<std::endl;
                        std::cout<<"请输入您要保存的存档位置(1~9)：";
                        std::getline(std::cin,input);
                        while(input.size()>1||input[0]<'1'||input[0]>'9'){
                            std::cout<<"您输入的格式有误。请重新输入"<<std::endl;
                            std::cout<<"请输入您要保存的存档位置(1~9)：";
                            std::getline(std::cin,input);
                        }
                    }
                    std::cout<<"保存成功！"<<std::endl;
                    continue;
                }
                //读盘
                else if(user_input=="l"){
                    bool is_save_exist=Filemanage::list_saves();
                    if(!is_save_exist){
                        std::cout<<"您没有存档可以读取。"<<std::endl;
                        continue;
                    }
                    std::cout<<"请输入您要读取的存档编号(0~9)：";
                    std::string input;std::getline(std::cin,input);
                    while(input.size()>1||input[0]<'0'||input[0]>'9'){
                        std::cout<<"您输入的格式有误。请重新输入。"<<std::endl;
                        std::cout<<"请输入您要读取的存档编号(0~9)：";
                        std::getline(std::cin,input);
                    }
                    int id=input[0]-'0';
                    while(!Filemanage::loadfile(gameboard,human_color,current_player,id)){
                            std::cout<<"请重新输入"<<std::endl;
                            std::cout<<"请输入您要读取的存档编号(0~9)：";
                            std::getline(std::cin,input);
                        while(input.size()>1||input[0]<'0'||input[0]>'9'){
                            std::cout<<"您输入的格式有误。请重新输入"<<std::endl;
                            std::cout<<"请输入您要读取的存档编号(0~9)：";
                            std::getline(std::cin,input);
                        }
                    }
                    std::cout<<"读取成功！"<<std::endl;
                    input_l=1;
                    break;
                }
                //暂停
                else if(user_input=="p"){
                    std::cout<<"========================================"<<std::endl;
                    std::cout<<"               游戏已暂停。              "<<std::endl;
                    std::cout<<"========================================"<<std::endl;
                    std::cout<<"按下回车键以继续游戏。"<<std::endl;
                    std::cin.get();
                }
                //结束
                else if(user_input=="e"){
                    std::cout<<"结束本局游戏，回到选择页面"<<std::endl;
                    return;
                }
                else {
                    try{
                        now_move=human.get_move(user_input,gameboard);
                        clear_screen();
                        std::cout<<"起点:"<<now_move.begin.y<<" "<<now_move.begin.x<<std::endl;
                        std::cout<<"终点:"<<now_move.end.y<<" "<<now_move.end.x<<std::endl;
                        std::cout<<"障碍物:"<<now_move.obstacle.y<<" "<<now_move.obstacle.x<<std::endl;
                        is_input_valid=1;
                    }catch(const std::exception &e){
                        std::cout<<"[!]您的输入有误："<<e.what()<<std::endl;
                    }
                }
            }
        }
        else {
            ai.start_time_reset();
            std::cout<<"ai正在思考······"<<std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(2));
            clear_screen();
            now_move=ai.get_move(gameboard,current_player);
            now_move.printmove();
        }
        if(input_l)continue;
        gameboard.make_move(now_move,current_player);
        current_player=-current_player;
    }
}
void gameloop(){
    std::cout<<"欢迎游玩亚马逊棋！"<<std::endl;
    while(1){
        //选择界面
        Board gameboard;
        bool is_continue=0;
        int human_color=1;
        int current_player=1;
        std::string input;
        std::cout<<"                         开始菜单                          "<<std::endl;
        std::cout<<"----------------------------------------------------------"<<std::endl;
        std::cout<<"请选择相应操作:"<<std::endl;
        std::cout<<"A:开始游戏"<<std::endl;
        std::cout<<"B:继续游戏"<<std::endl;
        std::cout<<"C:退出游戏"<<std::endl;
        std::getline(std::cin,input);
        while(input!="A"&&input!="B"&&input!="C"){
            std::cout<<"您的输入有误，请重新输入"<<std::endl;
            std::cout<<"请选择相应操作"<<std::endl;
            std::cout<<"A:开始游戏"<<std::endl;
            std::cout<<"B:继续游戏"<<std::endl;
            std::cout<<"C:退出游戏"<<std::endl;
            std::getline(std::cin,input);
        }
        if(input=="A"){
            gameboard.initialize();
            std::string select_color;
            std::cout<<"请选择您希望的落子顺序。"<<std::endl;
            std::cout<<"A: 您先手"<<std::endl;
            std::cout<<"B: AI先手"<<std::endl;
            std::getline(std::cin,select_color);
            while(select_color!="A"&&select_color!="B"){
                std::cout<<"您的输入有误，请重新输入。"<<std::endl;
                std::cout<<"请选择您希望的落子顺序。"<<std::endl;
                std::cout<<"A: 您先手"<<std::endl;
                std::cout<<"B: AI先手"<<std::endl;
                std::getline(std::cin,select_color);
            }
            if(select_color=="A")human_color=1;
            else human_color=-1;
            current_player=1;
        }
        if(input=="B"){
            is_continue=1;
            bool is_save_exist=Filemanage::list_saves();
            if(!is_save_exist){
                std::cout<<"您没有存档可以读取。"<<std::endl;
                continue;
            }
            std::cout<<"请输入您要读取的存档编号(0~9)：";
            std::string input;std::getline(std::cin,input);
            while(input.size()>1||input[0]<'0'||input[0]>'9'){
                std::cout<<"您输入的格式有误。请重新输入。"<<std::endl;
                std::cout<<"请输入您要读取的存档编号(1~9)：";
                std::getline(std::cin,input);
            }
            int id=input[0]-'0';
            while(!Filemanage::loadfile(gameboard,human_color,current_player,id)){
                    std::cout<<"请重新输入"<<std::endl;
                    std::cout<<"请输入您要读取的存档编号(0~9)：";
                    std::getline(std::cin,input);
                while(input.size()>1||input[0]<'0'||input[0]>'9'){
                    std::cout<<"您输入的格式有误。请重新输入"<<std::endl;
                    std::cout<<"请输入您要读取的存档编号(0~9)：";
                    std::getline(std::cin,input);
                }
            }
        }
        if(input=="C"){
            return;
        }
        play_a_game(gameboard,human_color,current_player,is_continue);
    }
}
int main(){
    SetConsoleOutputCP(CP_UTF8); 
    gameloop();
    return 0;
}
/*
2 0 4 2 5 1
*/