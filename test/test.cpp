#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert> // 用于断言
#include <cmath>
#include "board.h"
#include "move.h"
// --- Helper Functions for Test ---
#include "test_standards.h" // 将所有标准答案的计算函数放入一个单独的文件，保持整洁
// 自动化比较函数
bool tests_passed = true;
template <typename T>
void compare_grids(const std::string& title, const std::vector<std::vector<T>>& your_grid, const std::vector<std::vector<T>>& std_grid) {
    std::cout << "[TESTING] " << title << " ... ";
    if (your_grid.size() != std_grid.size() || your_grid[0].size() != std_grid[0].size()) {
        std::cout << "FAIL (Dimension mismatch)\n";
        tests_passed = false;
        return;
    }
    for (size_t i = 0; i < your_grid.size(); ++i) {
        for (size_t j = 0; j < your_grid[i].size(); ++j) {
            if (your_grid[i][j] != std_grid[i][j]) {
                std::cout << "FAIL\n";
                std::cout << "  -> Mismatch at (" << i << "," << j << "): Your value=" << your_grid[i][j] << ", Standard=" << std_grid[i][j] << std::endl;
                print_grid("Your Grid:", your_grid);
                print_grid("Standard Grid:", std_grid);
                tests_passed = false;
                return;
            }
        }
    }
    std::cout << "PASS\n";
}
void compare_doubles(const std::string& title, double your_val, double std_val, double tolerance = 1e-6) {
    std::cout << "[TESTING] " << title << " ... ";
    if (std::abs(your_val - std_val) > tolerance) {
        std::cout << "FAIL\n";
        std::cout << "  -> Your value=" << your_val << ", Standard=" << std_val << std::endl;
        tests_passed = false;
    } else {
        std::cout << "PASS\n";
    }
}
void compare_vectors(const std::string& title, const std::vector<double>& your_vec, const std::vector<double>& std_vec, double tolerance = 1e-6) {
    std::cout << "[TESTING] " << title << " ... ";
    if (your_vec.size() != std_vec.size()) {
        std::cout << "FAIL (Size mismatch: Your size=" << your_vec.size() << ", Standard=" << std_vec.size() << ")\n";
        tests_passed = false;
        return;
    }
    for (size_t i = 0; i < your_vec.size(); ++i) {
        if (std::abs(your_vec[i] - std_vec[i]) > tolerance) {
            std::cout << "FAIL\n";
            std::cout << "  -> Mismatch at index " << i << ": Your value=" << your_vec[i] << ", Standard=" << std_vec[i] << std::endl;
            tests_passed = false;
            return;
        }
    }
    std::cout << "PASS\n";
}
// 封装的测试函数
void run_all_tests_on_board(Board& board) {
    auto grid = board.get_grid();
    // 1. 获取标准答案
    auto std_min_queen_dist_a = min_dist_grid_std(1, grid, true);
    auto std_min_queen_dist_b = min_dist_grid_std(-1, grid, true);
    auto std_min_king_dist_a = min_dist_grid_std(1, grid, false);
    auto std_min_king_dist_b = min_dist_grid_std(-1, grid, false);
    auto std_n_grid = calculate_N_grid_std(grid);
    auto std_w_value = calculate_w_std(std_min_queen_dist_a, std_min_queen_dist_b);
    auto std_alpha_a = calculate_alpha_std(1, grid, std_n_grid);
    auto std_alpha_b = calculate_alpha_std(-1, grid, std_n_grid);
    // 2. 获取你的函数的计算结果 (因为 private 已改成 public, 我们可以直接调用)
    auto your_min_queen_dist_a = board.min_queen_dist_grid(1);
    auto your_min_queen_dist_b = board.min_queen_dist_grid(-1);
    auto your_min_king_dist_a = board.min_king_dist_grid(1);
    auto your_min_king_dist_b = board.min_king_dist_grid(-1);
    const auto& your_n_grid = board.N; // 直接访问 N 成员
    auto your_w_value = board.calc_w(your_min_queen_dist_a, your_min_queen_dist_b);
    auto your_alpha_a = board.calc_alpha(1);
    auto your_alpha_b = board.calc_alpha(-1);
    // 3. 自动比较
    compare_grids("Min Queen Distance ('a')", your_min_queen_dist_a, std_min_queen_dist_a);
    compare_grids("Min Queen Distance ('b')", your_min_queen_dist_b, std_min_queen_dist_b);
    compare_grids("Min King Distance ('a')", your_min_king_dist_a, std_min_king_dist_a);
    compare_grids("Min King Distance ('b')", your_min_king_dist_b, std_min_king_dist_b);
    compare_grids("N Matrix", your_n_grid, std_n_grid);
    compare_doubles("W Value", your_w_value, std_w_value);
    compare_vectors("Alpha Values ('a')", your_alpha_a, std_alpha_a);
    compare_vectors("Alpha Values ('b')", your_alpha_b, std_alpha_b);
}
int main() {
    Board board;
    std::vector<Move> moves_history; // 用于记录走法，方便撤销
    std::vector<int> colors_history;   // 记录走棋的玩家

    // =================================================================
    // TEST PHASE 1: INITIAL STATE (Amazons Opening)
    // =================================================================
    std::cout << "===== TEST PHASE 1: INITIAL STATE =====\n";
    board.initialize(); // 假设这会设置标准的Amazons开局
    std::cout << "--- Initial Board State ---\n";
    board.print_board();
    run_all_tests_on_board(board);
    if (!tests_passed) {
        std::cerr << "CRITICAL: Initial state test failed. Aborting further tests.\n";
        return 1;
    }


    // =================================================================
    // TEST PHASE 2: AFTER 5 MOVES
    // =================================================================
    std::cout << "\n===== TEST PHASE 2: MAKING 5 MOVES =====\n";
    
    // Move 1 (Player 1, white, moves from (0,3))
    Move move1 = {{0, 2}, {1, 2}, {2, 2}};
    board.make_move(move1, 1);
    moves_history.push_back(move1);
    colors_history.push_back(1);
    std::cout << "Made move 1: Player 1 from (0,2) to (1,2), obstacle at (2,2)\n";
    board.print_board();
    // Move 2 (玩家 -1, 'b'): 从 (5,0) 垂直移动到 (4,0), 在 (3,0) 放置障碍
    Move move2 = {{5, 0}, {4, 0}, {3, 0}};
    board.make_move(move2, -1);
    moves_history.push_back(move2);
    colors_history.push_back(-1);
    std::cout << "Made move 2: Player -1 from (5,0) to (4,0), obstacle at (3,0)\n";
    board.print_board();
    // Move 3 (玩家 1, 'a'): 从 (2,7) 对角移动到 (3,6), 在 (4,5) 放置障碍
    Move move3 = {{2, 7}, {3, 6}, {4, 5}};
    board.make_move(move3, 1);
    moves_history.push_back(move3);
    colors_history.push_back(1);
    std::cout << "Made move 3: Player 1 from (2,7) to (3,6), obstacle at (4,5)\n";
    board.print_board();
    // Move 4 (玩家 -1, 'b'): 从 (7,5) 水平移动到 (7,4), 在 (7,3) 放置障碍
    Move move4 = {{7, 5}, {7, 4}, {7, 3}};
    board.make_move(move4, -1);
    moves_history.push_back(move4);
    colors_history.push_back(-1);
    std::cout << "Made move 4: Player -1 from (7,5) to (7,4), obstacle at (7,3)\n";
    board.print_board();
    // Move 5 (玩家 1, 'a'): 从 (0,5) 对角移动到 (1,6), 在 (0,7) 放置障碍
    Move move5 = {{0, 5}, {1, 6}, {0, 7}};
    board.make_move(move5, 1);
    moves_history.push_back(move5);
    colors_history.push_back(1);
    std::cout << "Made move 5: Player 1 from (0,5) to (1,6), obstacle at (0,7)\n";
    board.print_board();
    
    std::cout << "\n--- Board state after 5 moves ---\n";
    board.print_board();
    run_all_tests_on_board(board);


    // =================================================================
    // TEST PHASE 3: AFTER UNDOING 2 MOVES
    // =================================================================
    std::cout << "\n===== TEST PHASE 3: UNDOING 2 MOVES =====\n";

    // Undo Move 5
    if (!moves_history.empty()) {
        board.undo_move(moves_history.back(), colors_history.back());
        std::cout << "Undid move 5: Player " << colors_history.back() << " reverse from (" 
                  << moves_history.back().begin.x << "," << moves_history.back().begin.y << ")\n";
        moves_history.pop_back();
        colors_history.pop_back();
    }

    // Undo Move 4
    if (!moves_history.empty()) {
        board.undo_move(moves_history.back(), colors_history.back());
        std::cout << "Undid move 4: Player " << colors_history.back() << " reverse from (" 
                  << moves_history.back().begin.x << "," << moves_history.back().begin.y << ")\n";
        moves_history.pop_back();
        colors_history.pop_back();
    }

    std::cout << "\n--- Board state after undoing 2 moves (should match state after 3 moves) ---\n";
    board.print_board();
    run_all_tests_on_board(board);
    

    // =================================================================
    // FINAL RESULT
    // =================================================================
    std::cout << "\n==============================================\n";
    if (tests_passed) {
        std::cout << "    ALL TEST PHASES COMPLETED SUCCESSFULLY!\n";
    } else {
        std::cout << "    SOME TESTS FAILED. Please check the logs above.\n";
    }
    std::cout << "==============================================\n";

    return tests_passed ? 0 : 1; // 如果有测试失败，返回非0状态码
}
