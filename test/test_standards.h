#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> // 用于格式化输出
#include <string>
#include <algorithm>
#include <cmath>
// 包含你自己的 Board 和 Move 定义
#include "board.h"
#include "move.h" // 需要包含 move.h 来使用 Move 结构体

// --- Helper Functions for Test ---
const int INF = 1e9;
// 打印2D矩阵的模板函数
template <typename T>
void print_grid(const std::string& title, const std::vector<std::vector<T>>& grid) {
    std::cout << "--- " << title << " ---" << std::endl;
    for (const auto& row : grid) {
        for (const auto& val : row) {
            if (val == INF) {
                std::cout << std::setw(5) << "INF";
            } else {
                std::cout << std::setw(5) << val;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
// --- Standard Calculation Logic (Re-implementation of your private methods) ---
// 这些函数独立于你的 Board 类，用于生成标准答案
bool is_in_board_std(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}
std::vector<std::vector<int>> queen_dist_grid_std(const Position& pos, const std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> dist(8, std::vector<int>(8, INF));
    if (!is_in_board_std(pos.x, pos.y)) return dist;
    
    std::vector<int> way_x = {1, -1, 0, 0, 1, -1, 1, -1};
    std::vector<int> way_y = {0, 0, 1, -1, 1, -1, -1, 1};
    dist[pos.x][pos.y] = 0;
    std::queue<Position> q;
    q.push(pos);
    while (!q.empty()) {
        Position cur = q.front();
        q.pop();
        for (int way_id = 0; way_id < 8; ++way_id) {
            for (int k = 1; ; ++k) {
                int nx = cur.x + k * way_x[way_id];
                int ny = cur.y + k * way_y[way_id];
                if (!is_in_board_std(nx, ny) || grid[nx][ny] != 0) break;
                if (dist[nx][ny] == INF) {
                    dist[nx][ny] = dist[cur.x][cur.y] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
    return dist;
}
std::vector<std::vector<int>> king_dist_grid_std(const Position& pos, const std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> dist(8, std::vector<int>(8, INF));
    if (!is_in_board_std(pos.x, pos.y)) return dist;
    std::vector<int> way_x = {1, -1, 0, 0, 1, -1, 1, -1};
    std::vector<int> way_y = {0, 0, 1, -1, 1, -1, -1, 1};
    dist[pos.x][pos.y] = 0;
    std::queue<Position> q;
    q.push(pos);
    while (!q.empty()) {
        Position cur = q.front();
        q.pop();
        for (int way_id = 0; way_id < 8; ++way_id) {
            int nx = cur.x + way_x[way_id];
            int ny = cur.y + way_y[way_id];
            if (is_in_board_std(nx, ny) && grid[nx][ny] == 0 && dist[nx][ny] == INF) {
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                q.push({nx, ny});
            }
        }
    }
    return dist;
}
std::vector<std::vector<int>> min_dist_grid_std(int color, const std::vector<std::vector<int>>& grid, bool is_queen) {
    std::vector<std::vector<int>> min_dist(8, std::vector<int>(8, INF));
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c] == color) {
                std::vector<std::vector<int>> current_dist = is_queen ? 
                    queen_dist_grid_std({r, c}, grid) :
                    king_dist_grid_std({r, c}, grid);
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        min_dist[i][j] = std::min(min_dist[i][j], current_dist[i][j]);
                    }
                }
            }
        }
    }
    return min_dist;
}
std::vector<std::vector<int>> calculate_N_grid_std(const std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> N(8, std::vector<int>(8, 0));
    std::vector<int> way_x = {1, -1, 0, 0, 1, -1, 1, -1};
    std::vector<int> way_y = {0, 0, 1, -1, 1, -1, -1, 1};
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c] != 0) {
                N[r][c] = 0;
                continue;
            }
            int count = 0;
            for (int i = 0; i < 8; ++i) {
                if (is_in_board_std(r + way_x[i], c + way_y[i]) && grid[r + way_x[i]][c + way_y[i]] == 0) {
                    count++;
                }
            }
            N[r][c] = count;
        }
    }
    return N;
}
// 标准的 pow_2 生成和计算函数
auto get_pow_calculator() {
    static std::vector<double> pow_2;
    if (pow_2.empty()) {
        pow_2.resize(40, 0);
        pow_2[0] = 1;
        for (int i = 1; i < 39; ++i) {
            pow_2[i] = pow_2[i - 1] / 2.0;
        }
    }
    return [=](int num) -> double {
        if (num < 0 || num >= pow_2.size()) return 0.0;
        return pow_2[num];
    };
}
std::vector<double> calculate_alpha_std(int color, const std::vector<std::vector<int>>& grid, const std::vector<std::vector<int>>& N_grid) {
    auto calc_pow_std = get_pow_calculator();
    
    std::vector<double> alpha;
    std::vector<Position> start_positions;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c] == color) {
                start_positions.push_back({r, c});
            }
        }
    }
    alpha.resize(start_positions.size(), 0.0);
    
    std::vector<int> way_x = {1, -1, 0, 0, 1, -1, 1, -1};
    std::vector<int> way_y = {0, 0, 1, -1, 1, -1, -1, 1};
    for (size_t i = 0; i < start_positions.size(); ++i) {
        Position cur = start_positions[i];
        for (int way_id = 0; way_id < 8; ++way_id) {
            int cnt = 0;
            for (int k = 1; ; ++k) {
                int nx = cur.x + k * way_x[way_id];
                int ny = cur.y + k * way_y[way_id];
                if (!is_in_board_std(nx, ny) || grid[nx][ny] != 0) break;
                cnt++;
                alpha[i] += N_grid[nx][ny] * calc_pow_std(cnt - 1);
            }
        }
    }
    return alpha;
}
// 标准的 W 值计算
double calculate_w_std(const std::vector<std::vector<int>>& dist1, const std::vector<std::vector<int>>& dist2) {
    auto calc_pow_std = get_pow_calculator();
    double w = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // 如果一个为INF而另一个不是，差值会很大，calc_pow会返回0，这符合逻辑
            w += calc_pow_std(std::abs(dist1[i][j] - dist2[i][j]));
        }
    }
    return w;
}
// 封装的测试函数，可以对任何棋盘状态进行测试
void run_all_tests_for_grid(const std::vector<std::vector<int>>& grid) {
    // Queen 距离
    auto min_queen_dist_a = min_dist_grid_std(1, grid, true);
    print_grid("Standard Calc: Min Queen Distance (Player 'a')", min_queen_dist_a);
    auto min_queen_dist_b = min_dist_grid_std(-1, grid, true);
    print_grid("Standard Calc: Min Queen Distance (Player 'b')", min_queen_dist_b);
    // King 距离
    auto min_king_dist_a = min_dist_grid_std(1, grid, false);
    print_grid("Standard Calc: Min King Distance (Player 'a')", min_king_dist_a);
    auto min_king_dist_b = min_dist_grid_std(-1, grid, false);
    print_grid("Standard Calc: Min King Distance (Player 'b')", min_king_dist_b);
    
    // N 值
    auto n_grid = calculate_N_grid_std(grid);
    print_grid("Standard Calc: N Values", n_grid);
    // W 值
    double w_value = calculate_w_std(min_queen_dist_a, min_queen_dist_b);
    std::cout << "--- Standard Calc: W Value ---\n";
    std::cout << std::fixed << std::setprecision(6) << w_value << std::endl << std::endl;
    // Alpha 值
    std::cout << "--- Standard Calc: Alpha Values ---\n";
    auto alpha_a = calculate_alpha_std(1, grid, n_grid);
    std::cout << "Player 'a' alpha: ";
    for(double val : alpha_a) std::cout << std::fixed << std::setprecision(4) << val << " ";
    std::cout << std::endl;
    auto alpha_b = calculate_alpha_std(-1, grid, n_grid);
    std::cout << "Player 'b' alpha: ";
    for(double val : alpha_b) std::cout << std::fixed << std::setprecision(4) << val << " ";
    std::cout << std::endl << std::endl;
}