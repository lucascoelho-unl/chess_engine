#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include "order.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

namespace chess_engine {
namespace search {

const int INF = std::numeric_limits<int>::max();
const int NEG_INF = std::numeric_limits<int>::min();

std::pair<int, moves::Move> negamax(int depth, int alpha, int beta, piece::Color color, game_state::Game_State &game_state) {
    // Base case: If the game is over or max depth is reached, return evaluation
    if (game_state.is_game_over()) {
        return {-evaluate::evaluate(color, game_state) * (depth + 1), moves::Move()};
    }

    if (depth == 0) {
        return {evaluate::evaluate(color, game_state), moves::Move()};
    }

    int maxEval = NEG_INF;
    moves::Move best_move;
    std::vector<moves::Move> possible_moves = order::order_moves(moves::generate_legal_moves(color, game_state), game_state);

    for (const auto &move : possible_moves) {
        game_state.make_move(move);
        int eval = -negamax(depth - 1, -beta, -alpha, utils::opposite_color(color), game_state).first;
        game_state.unmake_move();

        if (eval > maxEval) {
            maxEval = eval;
            best_move = move;
        }

        alpha = std::max(alpha, eval);
        if (alpha >= beta) {
            break;
        }
    }

    return {maxEval, best_move};
}

moves::Move find_best_move(int depth, piece::Color color, game_state::Game_State &game_state) {
    // Find the best move using negamax with alpha-beta pruning
    std::pair<int, moves::Move> result = negamax(depth, NEG_INF, INF, color, game_state);
    return result.second;
}

} // namespace search
} // namespace chess_engine