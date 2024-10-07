#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include "order.h"
#include "transposition.h"
#include "zobrist.h"
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

// Declare the global transposition table (defined in main.cpp)
extern transposition::TranspositionTable tt;

std::pair<int, moves::Move> negamax(int depth, int alpha, int beta, piece::Color color, game_state::GameState &game_state) {
    uint64_t hash = zobrist::compute_hash(game_state);

    // Probe the transposition table
    int tt_score;
    transposition::NodeType tt_type;
    moves::Move tt_move;
    if (tt.probe(hash, depth, tt_score, tt_type, tt_move)) {
        if (tt_type == transposition::NodeType::EXACT) {
            return {tt_score, tt_move};
        } else if (tt_type == transposition::NodeType::ALPHA && tt_score <= alpha) {
            return {alpha, tt_move};
        } else if (tt_type == transposition::NodeType::BETA && tt_score >= beta) {
            return {beta, tt_move};
        }
    }

    // Base case: If the game is over or max depth is reached, return evaluation
    if (game_state.is_game_over()) {
        return {evaluate::evaluate(color, game_state) * (depth + 1), moves::Move()};
    }

    if (depth == 0) {
        return {evaluate::evaluate(color, game_state), moves::Move()};
    }

    int max_eval = NEG_INF;
    moves::Move best_move;
    std::vector<moves::Move> possible_moves = order::order_moves(moves::generate_legal_moves(color, game_state), game_state);

    // Use the TT move if available
    if (!tt_move.is_null()) {
        auto it = std::find(possible_moves.begin(), possible_moves.end(), tt_move);
        if (it != possible_moves.end()) {
            std::iter_swap(possible_moves.begin(), it);
        }
    }

    for (const auto &move : possible_moves) {
        game_state.make_move(move);
        int eval = -negamax(depth - 1, -beta, -alpha, utils::opposite_color(color), game_state).first;
        game_state.unmake_move();

        if (eval > max_eval) {
            max_eval = eval;
            best_move = move;
        }

        alpha = std::max(alpha, eval);
        if (alpha >= beta) {
            break;
        }
    }

    // Store the result in the transposition table
    transposition::NodeType node_type;
    if (max_eval <= alpha) {
        node_type = transposition::NodeType::ALPHA;
    } else if (max_eval >= beta) {
        node_type = transposition::NodeType::BETA;
    } else {
        node_type = transposition::NodeType::EXACT;
    }

    uint64_t tt_hash = hash;
    int tt_depth = depth;
    int tt_max_eval = max_eval;
    transposition::NodeType tt_node_type = node_type;
    moves::Move tt_best_move = best_move;

    // std::cout << "Hash: " << tt_hash << " - ";
    // std::cout << moves::to_string(tt_best_move) << std::endl;

    tt.store(tt_hash, tt_depth, tt_max_eval, tt_node_type, tt_best_move);

    return {max_eval, best_move};
}

moves::Move find_best_move(int depth, piece::Color color, game_state::GameState &game_state) {
    // Find the best move using negamax with alpha-beta pruning
    std::pair<int, moves::Move> result = negamax(depth, NEG_INF, INF, color, game_state);
    return result.second;
}

moves::Move calculate_best_move(const std::string &fen) {
    // Initialize a board with the given FEN
    game_state::GameState state = game_state::set_game_state(fen);

    // Use the search algorithm to find the best move
    moves::Move best_move = find_best_move(4, state.turn, state);

    // Convert the best move to a string (e.g., e2e4)
    return best_move;
}

} // namespace search
} // namespace chess_engine