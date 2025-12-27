#include "AIplayer.h"
#include <random>
#include <iostream>
#include <chrono>
#include <stdexcept>  
constexpr int TIME_LIMIT=1000;
AIplayer::AIplayer(int color):player_color(color),gen(std::random_device()()){
    history_table.resize(8*8*8*8*8*8,0);
    start_time=std::chrono::steady_clock::now();
}
Move AIplayer::get_move(Board now_board,int color){
    Move ret=minmax_strategy(now_board,color);
    return ret;
}
Move AIplayer::rand_strategy(const Board &now_board)const{
    vector<Move> psb_move=now_board.get_all_psb_move(player_color);
    int len=psb_move.size();
    std::uniform_int_distribution<> distr(0,len-1);
    int rand_number=distr(gen);
    return psb_move[rand_number];
}
bool AIplayer::is_reach_time_limit(){
    auto now_time=std::chrono::steady_clock::now();
    auto elapsed_time=std::chrono::duration_cast<std::chrono::milliseconds>(now_time-start_time).count();
    //std::cout<<"Elapsed time: "<<elapsed_time<<" ms"<<std::endl;
    return elapsed_time>=(TIME_LIMIT-140);
}
Move AIplayer::minmax_strategy(const Board &gameboard,int color){
    Board now_board=gameboard;
    search_cnt=0;
    Move best_move;
    Move current_best_move;
    int max_depth=0;
    vector<Move> psb_move=now_board.get_all_psb_move(color);
    double alpha=-1e9,beta=1e9;
    for(int depth=0;;depth++){
        alpha=-1e9;beta=1e9;
        best_move=current_best_move;   
        max_depth=depth;
        if(is_reach_time_limit())break;
        try{
            for(const auto &move:psb_move){
                search_cnt++;
                now_board.make_move(move,color);
                double score=-alpha_beta_search(now_board,-color,depth,-beta,-alpha);
                now_board.undo_move(move,color);
                if(stop_search){
                    break;
                }
                if(score>alpha){
                    alpha=score;
                    current_best_move=move;
                }
            }
            //current_best_move.printmove();
        }catch(std::runtime_error &e){
            break;
        }
        if(stop_search)break;
        //test_output
        std::cout<<"Depth "<<depth<<" completed. Current best move: ";
        std::cout<<"alpha: "<<alpha<<", beta: "<<beta<<std::endl;
        current_best_move.printmove();
        now_board=gameboard;
        now_board.make_move(current_best_move,color);
        now_board.print_board();
        now_board.calc_board_score(-color,true);
        now_board.undo_move(current_best_move,color);
        
    }
   /*------------------------greedy version------------------*/
   /*for(const auto &move:psb_move){
                //std::cout<<"Searching move: ";move.printmove(); 
                search_cnt++;
                now_board.make_move(move,color);
                double score=-alpha_beta_search(now_board,-color,0,-beta,-alpha);
                now_board.undo_move(move,color);
                if(score>alpha){
                    //std::cout<<"New best score: "<<score<<std::endl;
                    //std::cout<<"ID:"<<search_cnt<<std::endl; 
                    //move.printmove();
                    alpha=score;
                    best_move=move;
                }
            }*/
      
    std::cout<<"Max search depth: "<<max_depth<<std::endl;
    std::cout<<"Total nodes searched: "<<search_cnt<<std::endl;
    return best_move;
}
double AIplayer::alpha_beta_search(Board &now_board,const int &color,const int &dep,double alpha,double beta){
    search_cnt++;
    if(stop_search){
        throw std::runtime_error("Time limit reached");
    }
    if((search_cnt&7)==0){
        if(is_reach_time_limit()){
            stop_search=1;
            throw std::runtime_error("Time limit reached");
        }
    }
    if(dep==0){
        return color*now_board.calc_board_score(color,0);
    }
    double max_score=-1e9;
    vector<Move> psb_move=now_board.get_all_psb_move(color);
    if(psb_move.empty()){
        return -1e9;
    }
    std::sort(psb_move.begin(), psb_move.end(), 
        [this](const Move& a, const Move& b) {
            return history_table[a.get_hash()] > history_table[b.get_hash()];
        }
    );
    bool is_first_child=1;
    for(const auto &move:psb_move){
        now_board.make_move(move,color);
        double score;
        if(is_first_child){
            score=-alpha_beta_search(now_board,-color,dep-1,-beta,-alpha);
            is_first_child=0;
        }
        else{
            score=-alpha_beta_search(now_board,-color,dep-1,-alpha-1,-alpha);
            if(score>alpha&&score<beta){
                score=-alpha_beta_search(now_board,-color,dep-1,-beta,-alpha);
            }
        }
        now_board.undo_move(move,color);
        if(score>max_score){
            max_score=score;
        }
        if(score>alpha){
            alpha=score;
        }
        if(alpha>=beta){
            history_table[move.get_hash()]+=dep*dep;
            return alpha;
        }
    }
    return max_score;
}