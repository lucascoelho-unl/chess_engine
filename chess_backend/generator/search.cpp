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

// Add a global transposition table
transposition::TranspositionTable tt(64); // 64 MB table

std::pair<int, moves::Move> negamax(int depth, int alpha, int beta, piece::Color color, game_state::Game_State &game_state) {
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
        return {-evaluate::evaluate(color, game_state) * (depth + 1), moves::Move()};
    }

    if (depth == 0) {
        return {evaluate::evaluate(color, game_state), moves::Move()};
    }

    int maxEval = NEG_INF;
    moves::Move best_move;
    std::vector<moves::Move> possible_moves = order::order_moves(moves::generate_legal_moves(color, game_state), game_state);

    // If we have a TT move, move it to the front of the list
    if (!tt_move.is_null()) {
        auto it = std::find(possible_moves.begin(), possible_moves.end(), tt_move);
        if (it != possible_moves.end()) {
            std::rotate(possible_moves.begin(), it, it + 1);
        }
    }

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

    // Store the result in the transposition table
    transposition::NodeType node_type;
    if (maxEval <= alpha) {
        node_type = transposition::NodeType::ALPHA;
    } else if (maxEval >= beta) {
        node_type = transposition::NodeType::BETA;
    } else {
        node_type = transposition::NodeType::EXACT;
    }
    tt.store(hash, depth, maxEval, node_type, best_move);

    return {maxEval, best_move};
}

moves::Move find_best_move(int depth, piece::Color color, game_state::Game_State &game_state) {
    // Find the best move using negamax with alpha-beta pruning
    std::pair<int, moves::Move> result = negamax(depth, NEG_INF, INF, color, game_state);
    return result.second;
}

moves::Move calculate_best_move(const std::string &fen) {
    // Initialize a board with the given FEN
    game_state::Game_State state = game_state::set_game_state(fen);

    // Use the search algorithm to find the best move
    moves::Move best_move = find_best_move(4, state.turn, state);

    // Convert the best move to a string (e.g., e2e4)
    return best_move;
}

} // namespace search
} // namespace chess_engine