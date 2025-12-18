#include "filemanage.h"
#include <string>
#include <fstream>
#include <iostream>
bool Filemanage::savefile(const Board &gameboard,const int &human_color,const int &current_player,const int &savenumber){
    std::string filename="SAVE_"+std::to_string(savenumber);
    std::string data_folder="../data/"; 
    std::string full_path=data_folder+filename;
    std::ofstream file(full_path);
    if(!file.is_open()){
        std::cout<<"[!]无法创建文件！"<<std::endl;
        return 0;
    }
    file<<human_color<<std::endl;
    file<<current_player<<std::endl;
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++){
            file<<gameboard.get_color(i,j)<<" ";
        }
        file<<std::endl;
    }
    file.close();
    return 1;
}
bool Filemanage::loadfile(Board &gameboard,int &human_color,int &current_player,const int &savenumber){
    std::string filename="SAVE_"+std::to_string(savenumber);
    //std::cout<<filename<<std::endl;
    std::string data_folder="../data/"; 
    std::string full_path=data_folder+filename;
    std::ifstream file(full_path);
    if(!file.is_open()){
        std::cout<<"[!]存档文件不存在！"<<std::endl;
        return 0;
    }
    file>>human_color;
    file>>current_player;
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++){
            int color;
            file>>color;
            gameboard.add({i,j},color);
        }
    }
    file.close();
    return 1;
}
bool Filemanage::list_saves(){
    std::cout<<"已有的存档："<<std::endl;
    bool is_exist=0;
    for(int id=0;id<=9;id++){
        std::string filename="SAVE_"+std::to_string(id);
        std::string data_folder="../data/"; 
        std::string full_path=data_folder+filename;
        std::ifstream file(full_path);
        if(file.is_open()){
            std::cout<<filename;
            if(id==0)std::cout<<"(自动保存)";
            std::cout<<std::endl;
            is_exist=1;
        }
        file.close();
    }
    if(!is_exist){std::cout<<"无"<<std::endl;return 0;}
    return 1;
}