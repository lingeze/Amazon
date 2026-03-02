// Merged faithfully according to the user's explicit instructions.
// My previous failures were unacceptable. This version is fully controlled.

// --- Part 1: System Headers ---
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
// --- Part 2: Class Definitions (in user-specified order) ---
// --- From: position.h ---
class Position{
    public:
        int x;int y;
        Position();
        Position(int initial_x,int initial_y);
        bool operator==(const Position &other)const;
};

// --- From: move.h ---
class Move{
    public:
        Position begin;
        Position end;
        Position obstacle;
        Move();
        int get_hash()const;
        void printmove()const;
        Move(const Position &initial_begin, const Position &initial_end, const Position &initial_obstacle);   
};
// --- From: board.h ---
using std::vector;
class Board{
    public:
    Board();
    int get_rounds()const{return rounds;}
    void set_rounds(int r){rounds=r;}
    vector<vector<int>> get_grid()const;
    int get_color(const int &pos_x,const int &pos_y)const;
    bool can_reach(const Position &begin,const Position &end)const;
    bool is_in_board(const Position &pos)const;
    bool check_move(const Move &move,const int &color)const;
    void print_board()const;
    void make_move(const Move &move,const int &color);
    void undo_move(const Move &move,const int &color);
    bool is_game_over(const int &next_color)const;
    void add(const Position &pos,const int &color);
    void del(const Position &pos);
    void initialize();
    /*For AI*/
    vector<Position> get_start_position(int color)const;
    vector<Move> get_all_psb_move(int color)const;
    double calc_board_score(const int &next_color,bool need_output)const;
    static void initialize_zobrist_keys();
    uint64_t get_hash() const;
    void calculate_full_hash();
    /*For gui*/
    void clear(){
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                del({i,j});
            }
        }
    }
    bool is_valid_start(Position pos,int color)const;
    vector<Position> reach_positions(Position pos)const;
    private:
    int row,col;
    bool can_move(const Position &pos)const;
    double calc_f(double alpha)const;
    /*For AI*/
    int rounds;
    vector<vector<int>> N;
    void N_update(const Position &pos);
    void N_calculate(const Position &pos);
    vector<double> calc_alpha(const int &color)const;
    static vector<double> pow_2;
    mutable vector<vector<int>> grid;
    char transform(int i,int j)const;
    double calc_pow(const int &num)const{
        if(num>pow_2.size())return 0; 
        return pow_2[num];
    }
    static void pow_initialize(){
        if(pow_2.size()>0)return;
        pow_2.resize(40,0);pow_2[0]=1;
        for(int i=1;i<39;i++){
            pow_2[i]=pow_2[i-1]/2;
        }
    }    
    vector<vector<int>> queen_dist_grid(const Position &pos)const;
    vector<vector<int>> king_dist_grid(const Position &pos)const;
    vector<vector<int>> min_queen_dist_grid(const int &color)const;
    vector<vector<int>> min_king_dist_grid(const int &color)const;
    double calc_w(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const;
    double calc_t(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2,const int &color)const;
    double calc_c1(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const;
    double calc_c2(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const;
    uint64_t current_hash;
    static std::vector<std::vector<uint64_t>> piece_keys;
    static uint64_t turn_key;
};
// --- From: AIplayer.h ---
const size_t TT_SIZE = 1 << 20;
enum class EntryFlag {
    EXACT,
    LOWER_BOUND,
    UPPER_BOUND
};
struct TTEntry {
    uint64_t key = 0;
    int depth = -1;
    double score = 0.0;
    EntryFlag flag;
};
class AIplayer{
    public:
        AIplayer(int Color);
        Move get_move(Board now_board,int color);
        void start_time_reset(){
            start_time=std::chrono::steady_clock::now();
        }
        void clear_history_table(){
            history_table.assign(8*8*8*8*8*8,0);
        }
    private:
        //int history_table[8][8][8][8][8][8]; 
        vector<int> history_table;
        int search_cnt=0;
        std::chrono::steady_clock::time_point start_time;
        bool stop_search=0;
        int player_color;
        mutable std::mt19937 gen;
        Move rand_strategy(const Board &now_board)const;
        double alpha_beta_search(Board &now_board,const int &color,const int &dep,double alpha,double beta);
        Move minmax_strategy(const Board &gameboard,int color);
        bool is_reach_time_limit();
        std::vector<TTEntry> transposition_table;
};
// --- From: Humanplayer.h ---
class Humanplayer{
    public:
        Humanplayer(int color);
        Move get_move(std::string user_input,const Board &now_board)const;
        ~Humanplayer();
    private:
        int player_color;
        Position to_position(std::string x,std::string y)const;
};
// --- From: filemanage.h ---
class Filemanage{
    public:
    static bool list_saves();
    static bool savefile(const Board &gameboard,const int &human_color,const int &current_player,const int &savenumber);
    static bool loadfile(Board &gameboard,int &human_color,int &current_player,const int &savenumber);
    bool file_exists(const int &savenumber);
};
// --- Part 3: Implementations ---
// --- From: AIplayer.cpp ---
constexpr int TIME_LIMIT=1000;
AIplayer::AIplayer(int color):player_color(color),gen(std::random_device()()){
    history_table.resize(8*8*8*8*8*8,0);
    start_time=std::chrono::steady_clock::now();
    transposition_table.resize(TT_SIZE);
}
Move AIplayer::get_move(Board now_board,int color){
    //now_board.print_board();
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
    //std::cout<<"Now time"<<now_time<<" ms"<<std::endl;
    //std::cout<<"Elapsed time: "<<elapsed_time<<" ms"<<std::endl;
    return elapsed_time>=(TIME_LIMIT-150);
}
Move AIplayer::minmax_strategy(const Board &gameboard,int color){
    Board now_board=gameboard;
    now_board.calculate_full_hash(); 
    search_cnt=0;
    Move best_move;
    Move current_best_move;
    int max_depth=0;
    stop_search=0;
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
        /*std::cout<<"Depth "<<depth<<" completed. Current best move: ";
        std::cout<<"alpha: "<<alpha<<", beta: "<<beta<<std::endl;
        current_best_move.printmove();
        now_board=gameboard;
        now_board.make_move(current_best_move,color);
        now_board.print_board();
        now_board.calc_board_score(-color,true);
        now_board.undo_move(current_best_move,color);*/
        
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
      
    //std::cout<<"Max search depth: "<<max_depth<<std::endl;
    //std::cout<<"Total nodes searched: "<<search_cnt<<std::endl;
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
    double original_alpha = alpha;
    uint64_t current_hash = now_board.get_hash(); 
    TTEntry* tt_entry = &transposition_table[current_hash & (TT_SIZE - 1)];
    if (tt_entry->key == current_hash && tt_entry->depth >= dep) {
        switch (tt_entry->flag) {
            case EntryFlag::EXACT:
                return tt_entry->score;
            case EntryFlag::LOWER_BOUND:
                alpha = std::max(alpha, tt_entry->score);
                break;
            case EntryFlag::UPPER_BOUND:
                beta = std::min(beta, tt_entry->score);
                break;
        }
        if (alpha >= beta) {
            return tt_entry->score;
        }
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
            tt_entry->key = current_hash;
            tt_entry->score = beta;
            tt_entry->flag = EntryFlag::LOWER_BOUND;
            tt_entry->depth = dep;
            return alpha;
        }
    }
    if (!stop_search) {
        tt_entry->key = current_hash;
        tt_entry->depth = dep;
        if (max_score <= original_alpha) {
            tt_entry->flag = EntryFlag::UPPER_BOUND;
            tt_entry->score = max_score;
        } else {
            tt_entry->flag = EntryFlag::EXACT;
            tt_entry->score = max_score;
        }
    }
    return max_score;
}
// --- From: Humanplayer.cpp ---
Humanplayer::Humanplayer(int color):player_color(color){

}
Position Humanplayer::to_position(std::string x,std::string y)const{
    //std::cout<<x<<" "<<x.size()<<" "<<y<<" "<<y.size()<<std::endl;
    if(x.size()!=1||y.size()!=1)throw std::runtime_error("您的输入有误，请按照给定格式输入。");
    if(!isdigit(x[0])||!isdigit(y[0]))throw std::runtime_error("您的输入不是数字，请按照给定格式输入。");
    int numx=x[0]-'0',numy=y[0]-'0';
    return {numx,numy};
}
Move Humanplayer::get_move(std::string user_input,const Board &now_board)const{
   // now_board.print_board();
    std::stringstream ss(user_input);
    Move ret_move={{0,0},{0,0},{0,0}};
    Position begin,end,obstacle;
    std::string bgx,bgy,edx,edy,obx,oby;
    ss>>bgy>>bgx>>edy>>edx>>oby>>obx;
    //std::cout<<bgy<<bgx<<edy<<edx<<oby<<obx<<std::endl;
    if(bgx.empty()||bgy.empty()||edx.empty()||edy.empty()||obx.empty()||oby.empty()){
        throw std::runtime_error("输入过少，请输入三个坐标。");
    }
    std::string junk;
    ss>>junk;
    if(!junk.empty()){
        throw std::runtime_error("输入过多，请输入三个坐标。");
    }
    try{
        ret_move.begin=to_position(bgx,bgy);
        ret_move.end=to_position(edx,edy);
        ret_move.obstacle=to_position(obx,oby);
    }catch(const std::exception &e){
        throw;
    }
    if(!now_board.check_move(ret_move,player_color)){
        throw std::runtime_error("输入的行动不符合规则，请重新输入。");
    }    
    //std::cout<<"起点:"<<ret_move.begin.y<<" "<<ret_move.begin.x<<std::endl;
    //std::cout<<"终点:"<<ret_move.end.y<<" "<<ret_move.end.x<<std::endl;
    //std::cout<<"障碍物:"<<ret_move.obstacle.y<<" "<<ret_move.obstacle.x<<std::endl;
    return ret_move;
}
Humanplayer::~Humanplayer(){
    
}
// --- From: board.cpp ---
using std::vector;
const int INF=1000000000;
const double K=0.05;
const double value_t1_1=0.14;
const double value_t1_2=0.30;
const double value_t1_3=0.80;
const double value_t2_1=0.37;
const double value_t2_2=0.25;
const double value_t2_3=0.10;
const double value_c1_1=0.13;
const double value_c1_2=0.20;
const double value_c1_3=0.10;
const double value_c2_1=0.13;
const double value_c2_2=0.20;
const double value_c2_3=0.05;
const double value_m_1=0.20;
const double value_m_2=0.05;
const double value_m_3=0.0;
const int state_1_end=25;
const int state_2_end=40;
std::vector<std::vector<uint64_t>> Board::piece_keys;
uint64_t Board::turn_key;
std::vector<double> Board::pow_2; 
Board::Board():row(8),col(8),grid(row,vector<int>(col,0)),N(row,vector<int>(col,0)),rounds(0){
    pow_initialize(); 
    Board::initialize_zobrist_keys();
}
int piece_to_id(int piece_color) {
    if (piece_color == 1) return 0;  // 黑
    if (piece_color == -1) return 1; // 白
    return 2;                      // 障碍
}
vector<vector<int>> Board::get_grid()const{
    return grid;
}
int Board::get_color(const int &pos_x,const int &pos_y)const{
    return grid[pos_x][pos_y];
}
bool Board::check_move(const Move &move,const int &color)const{
    if(!is_in_board(move.begin)||!is_in_board(move.end)||!is_in_board(move.obstacle))return 0;
    if(grid[move.begin.x][move.begin.y]!=color)return 0;
    //if(!)return 0;
    grid[move.begin.x][move.begin.y]=0;
    bool ok=can_reach(move.begin,move.end)&&can_reach(move.end,move.obstacle);
    grid[move.begin.x][move.begin.y]=color;
    if(!ok)return 0;
    return 1;
}
void Board::add(const Position &pos,const int &color){
    grid[pos.x][pos.y]=color;
}
void Board::del(const Position &pos){
    grid[pos.x][pos.y]=0;
}
char Board::transform(int i,int j)const{
    if(grid[i][j]==0)return '.';
    else if(grid[i][j]==1)return 'a';
    else if(grid[i][j]==-1)return 'b';
    else if(grid[i][j]==2)return '#';
    return ' ';
}
void Board::print_board()const{
    std::cout<<"    ";
    for(int i=0;i<col;i++){
        std::cout<<"  "<<i<<" ";
    }
    std::cout<<std::endl;
    for(int i=0;i<row;i++){
        std::cout<<"    ";
        std::cout<<"+---+---+---+---+---+---+---+---+"<<std::endl;
        std::cout<<"  "<<i<<" ";
        for(int j=0;j<col;j++){
            std::cout<<"|";
            std::cout<<" ";
            std::cout<<transform(i,j);
            std::cout<<" ";
        }
        std::cout<<"|";
        std::cout<<std::endl;
    }
    std::cout<<"    +---+---+---+---+---+---+---+---+"<<std::endl;
    std::cout<<std::endl;
}
bool Board::is_in_board(const Position &pos)const{
    if(pos.x<0||pos.x>=row||pos.y<0||pos.y>=col)return 0;
    return 1;
}
bool Board::can_reach(const Position &begin,const Position &end)const{
    if(begin==end)return 0;
    if(grid[end.x][end.y])return 0;
    int minx=std::min(begin.x,end.x);
    int miny=std::min(begin.y,end.y);
    int maxx=std::max(begin.x,end.x);
    int maxy=std::max(begin.y,end.y);
    if((maxy-miny)==(maxx-minx)){
        int addx=((minx==begin.x)?1:-1);
        int addy=((miny==begin.y)?1:-1);
        for(int i=begin.x+addx,j=begin.y+addy;i!=end.x&&j!=end.y;i+=addx,j+=addy){
            if(grid[i][j])return 0;
        }
    }
    else if(minx==maxx){
        if(miny==begin.y){
            for(int i=miny+1;i<=maxy;i++){
                //std::cout<<bgx<<" "<<i<<endl;
                if(grid[minx][i]){/*std::cout<<bgx<<" "<<i<<std::endl;*/return 0;}
            }
        }
        else {
            for(int i=miny;i<=maxy-1;i++){
                //std::cout<<bgx<<" "<<i<<endl;
                if(grid[minx][i]){/*std::cout<<bgx<<" "<<i<<std::endl;*/return 0;}
            }
        }
    }
    else if(miny==maxy){
        if(minx==begin.x){
            for(int i=minx+1;i<=maxx;i++){
                if(grid[i][miny])return 0;
            }
        }
        else {
            for(int i=minx;i<=maxx-1;i++){
                if(grid[i][miny])return 0;
            }
        }        
    }
    else return 0;
    return 1;
}
void Board::make_move(const Move &move,const int &color){
    int from_sq = move.begin.x * col + move.begin.y;
    int to_sq = move.end.x * col + move.end.y;
    int obs_sq = move.obstacle.x * col + move.obstacle.y;
    int color_id = piece_to_id(color);
    int obs_id = piece_to_id(2);
    current_hash ^= piece_keys[color_id][from_sq];
    current_hash ^= piece_keys[color_id][to_sq];
    current_hash ^= piece_keys[obs_id][obs_sq];
    current_hash ^= turn_key;
    set_rounds(rounds+1);
    del(move.begin);
    add(move.end,color);
    add(move.obstacle,2);
    N_update(move.begin);
    N_update(move.end);
    N_update(move.obstacle);
}
void Board::undo_move(const Move &move,const int &color){
    int from_sq = move.begin.x * col + move.begin.y;
    int to_sq = move.end.x * col + move.end.y;
    int obs_sq = move.obstacle.x * col + move.obstacle.y;
    int color_id = piece_to_id(color);
    int obs_id = piece_to_id(2);
    current_hash ^= piece_keys[obs_id][obs_sq];
    current_hash ^= piece_keys[color_id][to_sq];
    current_hash ^= piece_keys[color_id][from_sq];
    current_hash ^= turn_key;
    set_rounds(rounds-1);
    del(move.end);
    del(move.obstacle);
    add(move.begin,color);
    N_update(move.begin);
    N_update(move.end);
    N_update(move.obstacle);
}
bool Board::is_game_over(const int &next_color)const{
    //vector<Position> chess_pieces;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(grid[i][j]==next_color)if(can_move({i,j}))return 0;
        }
    }
    return 1;
}
bool Board::can_move(const Position &pos)const{
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    for(int id=0;id<=7;id++){
        int nx=pos.x+way_x[id],ny=pos.y+way_y[id];
        if(!is_in_board({nx,ny}))continue;
        if(!grid[nx][ny])return 1;
        
    }
    return 0;
}
void Board::initialize(){
    add({0,2},1);
    add({2,0},1);
    add({0,5},1);
    add({2,7},1);
    add({5,0},-1);
    add({7,2},-1);
    add({5,7},-1);
    add({7,5},-1);
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            N_calculate({i,j});
        }
    }
}
vector<Position> Board::get_start_position(int color)const{
    vector<Position> start;
    for(int i=0;i<=7;i++){
        for(int j=0;j<=7;j++){
            if(grid[i][j]==color)start.push_back({i,j});
        }
    }
    return start;
}
vector<Move> Board::get_all_psb_move(int color)const{
    vector<Position> start=get_start_position(color);
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    vector<Position> end;
    vector<Move> psb_move;
    for(const auto &st:start){
        end.clear();
        for(int way_id=0;way_id<=7;way_id++){
            for(int nx=st.x+way_x[way_id],ny=st.y+way_y[way_id];
                nx<=7&&ny<=7&&nx>=0&&ny>=0;
                nx+=way_x[way_id],ny+=way_y[way_id]
            ){
                if(grid[nx][ny])break;
                end.push_back({nx,ny});
            }
        }
        for(const auto &ed:end){
            for(int way_id=0;way_id<=7;way_id++){
                for(int nx=ed.x+way_x[way_id],ny=ed.y+way_y[way_id];
                    nx<=7&&ny<=7&&nx>=0&&ny>=0;
                    nx+=way_x[way_id],ny+=way_y[way_id]
                ){
                    if(grid[nx][ny]&&grid[nx][ny]!=color)break;
                    if(grid[nx][ny]==color&&(nx!=st.x||ny!=st.y))break;
                    psb_move.push_back({st,ed,{nx,ny}});
                }
            }
        } 
        //std::cout<<end.size()<<std::endl;
    }
    return psb_move;
}
vector<vector<int>> Board::queen_dist_grid(const Position &pos)const{
    vector<vector<int>> dist(row,vector<int>(col,INF));
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    dist[pos.x][pos.y]=0;
    std::queue<Position> q;
    q.push(pos);
    while(!q.empty()){
        Position cur=q.front();
        q.pop();
        for(int way_id=0;way_id<=7;way_id++){
            for(int nx=cur.x+way_x[way_id],ny=cur.y+way_y[way_id];
                nx<row&&ny<col&&nx>=0&&ny>=0;
                nx+=way_x[way_id],ny+=way_y[way_id]
            ){
                if(grid[nx][ny])break;
                if(dist[nx][ny]==INF){
                    dist[nx][ny]=dist[cur.x][cur.y]+1;
                    q.push({nx,ny});
                }
            }
        }
    }
    return dist;
}
vector<vector<int>> Board::king_dist_grid(const Position &pos)const{
    vector<vector<int>> dist(row,vector<int>(col,INF));
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    dist[pos.x][pos.y]=0;
    std::queue<Position> q;
    q.push(pos);
    while(!q.empty()){
        Position cur=q.front();
        q.pop();
        for(int way_id=0;way_id<=7;way_id++){
            int nx=cur.x+way_x[way_id],ny=cur.y+way_y[way_id];
            if(!is_in_board({nx,ny}))continue;
            if(grid[nx][ny])continue;
            if(dist[nx][ny]==INF){
                dist[nx][ny]=dist[cur.x][cur.y]+1;
                q.push({nx,ny});
            }
        }
    }
    return dist;
}
vector<vector<int>> Board::min_queen_dist_grid(const int &color)const{
    vector<vector<int>> dist(row,vector<int>(col,INF));
    vector<Position> start=get_start_position(color);
    for(const auto &st:start){
        vector<vector<int>> cur_dist=queen_dist_grid(st);
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                dist[i][j]=std::min(dist[i][j],cur_dist[i][j]);
            }
        }
    }
    return dist;
}
vector<vector<int>> Board::min_king_dist_grid(const int &color)const{
    vector<vector<int>> dist(row,vector<int>(col,INF));
    vector<Position> start=get_start_position(color);
    for(const auto &st:start){
        vector<vector<int>> cur_dist=king_dist_grid(st);
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                dist[i][j]=std::min(dist[i][j],cur_dist[i][j]);
            }
        }
    }
    return dist;
}
double Board::calc_t(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2,const int &color)const{
   double t=0;
   for(int i=0;i<row;i++){
       for(int j=0;j<col;j++){
            if(dist1[i][j]==INF&&dist2[i][j]==INF)continue;
            if(dist1[i][j]==dist2[i][j]&&dist1[i][j]!=INF)t+=K*color;
            else if(dist1[i][j]<dist2[i][j])t++;
            else t--;
       }
   }
   return t;
}
double Board::calc_c1(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const{
    double c1=0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            c1+=calc_pow(dist1[i][j])-calc_pow(dist2[i][j]);
        }
    }
    return 2*c1;
}
double Board::calc_c2(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const{
    double c2=0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            c2+=std::min(1.0,std::max(-1.0,double(dist2[i][j]-dist1[i][j])/6.0));
        }
    }
    return c2;
}
double Board::calc_w(const vector<vector<int>> &dist1,const vector<vector<int>> &dist2)const{
    double w=0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(dist1[i][j]==INF&&dist2[i][j]==INF)continue;
            w+=calc_pow(std::abs(dist1[i][j]-dist2[i][j]));
        }
    }
    return w;
}
void print_dist(const vector<vector<int>> &dist){
    for(int i=0;i<dist.size();i++){
        for(int j=0;j<dist[0].size();j++){
            if(dist[i][j]==INF)std::cout<<"X ";
            else std::cout<<dist[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}
double Board::calc_board_score(const int &next_color,bool need_output)const{
    //print_board();//test
    vector<vector<int>> min_queen_dist1=min_queen_dist_grid(1);
    vector<vector<int>> min_queen_dist2=min_queen_dist_grid(-1);
    vector<vector<int>> min_king_dist1=min_king_dist_grid(1);
    vector<vector<int>> min_king_dist2=min_king_dist_grid(-1);
    //test_begin
    if(need_output){
    std::cout<<"rounds: "<<rounds<<std::endl;
    std::cout<<"Queen Dist 1:"<<std::endl;
    print_dist(min_queen_dist1);
    std::cout<<"Queen Dist 2:"<<std::endl;
    print_dist(min_queen_dist2);
    std::cout<<"King Dist 1:"<<std::endl;
    print_dist(min_king_dist1);
    std::cout<<"King Dist 2:"<<std::endl;
    print_dist(min_king_dist2); 
    }
    //test_end
    //double w=calc_w(min_queen_dist1,min_queen_dist2);
    double t1=calc_t(min_queen_dist1,min_queen_dist2,next_color);
    double t2=calc_t(min_king_dist1,min_king_dist2,next_color);
    double c1=calc_c1(min_queen_dist1,min_queen_dist2);
    double c2=calc_c2(min_king_dist1,min_king_dist2);
    //test_begin
    if(need_output){ 
    std::cout<<"t1: "<<t1<<std::endl;
    std::cout<<"t2: "<<t2<<std::endl;
    std::cout<<"c1: "<<c1<<std::endl;
    std::cout<<"c2: "<<c2<<std::endl;
    }
    //test_end
    vector<double> alpha1=calc_alpha(1);
    vector<double> alpha2=calc_alpha(-1);
    //test_begin
    if(need_output){
    std::cout<<"N Grid: "<<std::endl;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            std::cout<<N[i][j]<<" ";
        }
        std::cout<<std::endl;
    } 
    std::cout<<"Alpha 1: ";
    for(int i=0;i<4;i++)std::cout<<alpha1[i]<<" ";
    std::cout<<std::endl;
    std::cout<<"Alpha 2: ";
    for(int i=0;i<4;i++)std::cout<<alpha2[i]<<" ";
    std::cout<<std::endl;
    }
    //test_end
    double m=0;
    for(int i=0;i<4;i++){
        m+=calc_f(alpha2[i])-calc_f(alpha1[i]);
    }
    double score=0;
    if(need_output){    
        std::cout<<"m: "<<m<<std::endl;
    }
    if(rounds<=state_1_end){
        score = t1*value_t1_1 + t2*value_t2_1 + c1*value_c1_1 + c2*value_c2_1 + m*value_m_1;
    }
    else if(rounds<=state_2_end){
        score = t1*value_t1_2 + t2*value_t2_2 + c1*value_c1_2 + c2*value_c2_2 + m*value_m_2;
    }
    else{
        score = t1*value_t1_3 + t2*value_t2_3 + c1*value_c1_3 + c2*value_c2_3 + m*value_m_3;
    }
    //std::cout<<"Score: "<<score<<std::endl;//test 
    return score;
}
void Board::initialize_zobrist_keys(){
    if (!piece_keys.empty()) return;
    std::mt19937_64 gen(1337); 
    std::uniform_int_distribution<uint64_t> distrib;
    piece_keys.resize(3, std::vector<uint64_t>(64));
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 64; ++j) {
            piece_keys[i][j] = distrib(gen);
        }
    }
    turn_key = distrib(gen);
    return;
}

uint64_t Board::get_hash() const{
    return current_hash;
}
void Board::calculate_full_hash(){
    current_hash = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (grid[i][j] != 0) {
                int piece_idx = piece_to_id(grid[i][j]);
                int square_idx = i * col + j;
                current_hash ^= piece_keys[piece_idx][square_idx];
            }
        }
    }
}
void Board::N_update(const Position &pos)
{
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1,0};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1,0};
    for(int way_id=0;way_id<=8;way_id++){
        int nx=pos.x+way_x[way_id],ny=pos.y+way_y[way_id];
        if(!is_in_board({nx,ny}))continue;
        N_calculate({nx,ny});
    }
}
void Board::N_calculate(const Position &pos){
    if(grid[pos.x][pos.y]){
        N[pos.x][pos.y]=0;
        return;
    }
    int cnt=0;
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    for(int way_id=0;way_id<=7;way_id++){
        int nx=pos.x+way_x[way_id],ny=pos.y+way_y[way_id];
        if(!is_in_board({nx,ny}))continue;
        if(!grid[nx][ny])cnt++;
    }
    N[pos.x][pos.y]=cnt;
}
vector<double> Board::calc_alpha(const int &color)const{
    vector<double> alpha(4,0);
    vector<Position> start=get_start_position(color);
    for(int i=0;i<4;i++){
        Position cur=start[i];
        static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
        static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
        for(int way_id=0;way_id<=7;way_id++){
            int cnt=0;
            for(int nx=cur.x+way_x[way_id],ny=cur.y+way_y[way_id];
                nx<row&&ny<col&&nx>=0&&ny>=0;
                nx+=way_x[way_id],ny+=way_y[way_id]
            ){
                cnt++;
                if(grid[nx][ny])break;
                alpha[i]+=N[nx][ny]*1.0*calc_pow(cnt-1);
            }
        }   
    }
    return alpha;
}
double Board::calc_f(double alpha)const{
    // ALPHA_HIGH_THRESHOLD: 在此alpha值以上，我们认为棋子是安全的，没有机动性问题。
    constexpr double ALPHA_HIGH_THRESHOLD = 20.0;
    // ALPHA_LOW_THRESHOLD: 在此alpha值以下，我们认为棋子“几乎被困”，需要施加重罚。
    constexpr double ALPHA_LOW_THRESHOLD = 5.0;
    // BASE_PENALTY_MULTIPLIER: 基础惩罚系数。
    constexpr double BASE_PENALTY_MULTIPLIER = 5.0;
    // SUPER_PENALTY_FACTOR: 当棋子“完全被困”时，在基础惩罚上额外乘以的系数。
    constexpr double SUPER_PENALTY_FACTOR = 10.0; 
    constexpr double ADJUCT_MULTIPLIER = 20.0; 
    double g_alpha = 0.0;
    if (alpha >=ALPHA_HIGH_THRESHOLD) {
        g_alpha = 0.0;
    } else if (alpha >ALPHA_LOW_THRESHOLD) {
        // --- 情况2: 棋子机动性受限，但不严重 (5 < alpha < 20) ---
        // 我们使用一个平滑的线性函数，从阈值处的0惩罚过渡到低阈值处的基础惩罚。
        // (alpha - low) / (high - low) 是一个从 0 到 1 的插值因子。
        // 我们用 1.0 减去它，实现一个从 1 到 0 的平滑下降。
        double factor = (alpha -ALPHA_LOW_THRESHOLD) / (ALPHA_HIGH_THRESHOLD -ALPHA_LOW_THRESHOLD);  
        g_alpha =BASE_PENALTY_MULTIPLIER * (1.0 - factor);
    } else {
        double base_penalty =BASE_PENALTY_MULTIPLIER;
        // 超级惩罚：当alpha从5降到0时，这部分惩罚会急剧增加。
        // (low - alpha) / low 是一个从 0 到 1 的插值因子，alpha越小，它越接近1。
        double super_penalty =BASE_PENALTY_MULTIPLIER * 
                            SUPER_PENALTY_FACTOR * 
                            (ALPHA_LOW_THRESHOLD - std::max(0.0, alpha)) *
                            (ALPHA_LOW_THRESHOLD - std::max(0.0, alpha)) /ALPHA_LOW_THRESHOLD;
        g_alpha = base_penalty + super_penalty;
    }
    return g_alpha/ADJUCT_MULTIPLIER;
}

bool Board::is_valid_start(Position pos,int color) const{
    if(!is_in_board(pos))return 0;
    return grid[pos.x][pos.y]==color;
}

vector<Position> Board::reach_positions(Position pos) const{
    vector<Position> ret;
    static vector<int> way_x={1,-1,0,0,1,-1,1,-1};
    static vector<int> way_y={0,0,1,-1,1,-1,-1,1};
    for(int id=0;id<=7;id++){
        for(int nx=pos.x+way_x[id],ny=pos.y+way_y[id];;nx+=way_x[id],ny+=way_y[id]){
            if(!is_in_board({nx,ny}))break;
            if(grid[nx][ny])break;
            ret.push_back({nx,ny});
        }
    }
    return ret;
}

// --- From: filemanage.cpp ---
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
bool Filemanage::file_exists(const int &savenumber){
    std::string filename="SAVE_"+std::to_string(savenumber);
    std::string data_folder="../data/";
    std::string full_path=data_folder+filename;
    std::ifstream file(full_path);
    if(file.is_open()){
        file.close();
        return true;
    }
    return false;
}
bool Filemanage::list_saves()
{
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
// --- From: move.cpp ---
Move::Move(){

}
Move::Move(const Position &initial_begin, const Position &initial_end,
const Position &initial_obstacle):
begin(initial_begin),end(initial_end),obstacle(initial_obstacle){
    
}
void Move::printmove()const{
    std::cout<<"起点:"<<begin.y<<" "<<begin.x<<std::endl;
    std::cout<<"终点:"<<end.y<<" "<<end.x<<std::endl;
    std::cout<<"障碍物:"<<obstacle.y<<" "<<obstacle.x<<std::endl;
}
int Move::get_hash()const{
    return begin.x + begin.y * 8 + end.x * 64 + end.y * 512 + obstacle.x * 4096 + obstacle.y * 32768;
}
// --- From: position.cpp ---
Position::Position(): x(0),y(0){

}
Position::Position(int initial_x,int initial_y): x(initial_x),y(initial_y){

}
bool Position::operator==(const Position &other)const{
    return (x==other.x)&&(y==other.y);
} 

// --- Part 4: Main Entry Point ---
// --- From: bot.cpp ---
int turn_id;
int main(){
    Board gameboard;
    gameboard.initialize();
    int my_color=-1;
    Move move;
    std::cin>>turn_id;
    std::cin>>move.begin.y>>move.begin.x>>move.end.y>>move.end.x>>move.obstacle.y>>move.obstacle.x;
    if(move.begin.y<0)my_color=1;
    else gameboard.make_move(move,-my_color);
    AIplayer ai(my_color);
    int now_color=my_color;
    for(int i=1;i<=turn_id-1;i++){
        std::cin>>move.begin.y>>move.begin.x>>move.end.y>>move.end.x>>move.obstacle.y>>move.obstacle.x;
        gameboard.make_move(move,now_color);
        if(i!=turn_id)
        std::cin>>move.begin.y>>move.begin.x>>move.end.y>>move.end.x>>move.obstacle.y>>move.obstacle.x;
        gameboard.make_move(move,-now_color);
    }
    Move nxt_move=ai.get_move(gameboard,my_color);
    std::cout<<nxt_move.begin.y<<" "<<nxt_move.begin.x<<" ";
    std::cout<<nxt_move.end.y<<" "<<nxt_move.end.x<<" ";
    std::cout<<nxt_move.obstacle.y<<" "<<nxt_move.obstacle.x<<" ";
    
    return 0;
}
