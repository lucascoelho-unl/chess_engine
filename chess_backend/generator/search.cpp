#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include <algorithm>
#include <chrono> // For timing
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

std::vector<moves::Move> order_moves(const std::vector<moves::Move> &moves, const game_state::Game_State &game_state) {
    std::vector<std::pair<int, moves::Move>> scored_moves;
    scored_moves.reserve(moves.size());

    for (const auto &move : moves) {
        int score = 0;

        // Prioritize captures
        if (move.move_type == moves::Type::CAPTURE) {
            score += 9000;
            // Prioritize capturing higher value pieces
            score += 100 * evaluate::piece_value(game_state.get_board().get_piece_type(move.to, utils::opposite_color(move.color)));
        }

        // Prioritize promotions
        if (move.move_type == moves::Type::PROMOTION) {
            score += 10000;
        }

        // Additional move ordering heuristics
        // Encourage central control
        if (move.to >= 27 && move.to <= 36) { // Central squares e4, e5, d4, d5
            score += 500;
        }

        scored_moves.emplace_back(score, move);
    }

    // Sort moves by score in descending order
    std::sort(scored_moves.begin(), scored_moves.end(), std::greater<>());

    std::vector<moves::Move> ordered_moves;
    for (const auto &scored_move : scored_moves) {
        ordered_moves.push_back(scored_move.second);
    }

    return ordered_moves;
}

int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer, game_state::Game_State &game_state) {
    // Base case: If game over or max depth reached, return evaluation
    if (game_state.is_game_over()) {
        return evaluate::evaluate(game_state) * (depth + 1);
    }

    if (depth == 0) {
        return evaluate::evaluate(game_state);
    }

    if (isMaximizingPlayer) {
        int maxEval = NEG_INF;
        std::vector<moves::Move> possible_moves = order_moves(moves::generate_legal_moves(piece::Color::WHITE, game_state), game_state);

        for (const auto &move : possible_moves) {
            game_state.make_move(move);
            int eval = minimax(depth - 1, alpha, beta, false, game_state);
            game_state.unmake_move();
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INF;
        std::vector<moves::Move> possible_moves = order_moves(moves::generate_legal_moves(piece::Color::BLACK, game_state), game_state);

        for (const auto &move : possible_moves) {
            game_state.make_move(move);
            int eval = minimax(depth - 1, alpha, beta, true, game_state);
            game_state.unmake_move();
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

moves::Move find_best_move(int depth, piece::Color color, game_state::Game_State &game_state) {
    int best_eval = (color == piece::Color::WHITE) ? NEG_INF : INF;
    moves::Move best_move;
    int alpha = NEG_INF;
    int beta = INF;

    std::vector<moves::Move> possible_moves = order_moves(moves::generate_legal_moves(color, game_state), game_state);
    bool is_maximizing = (color == piece::Color::WHITE);

    for (const auto &move : possible_moves) {
        game_state.make_move(move);
        int move_eval = minimax(depth - 1, alpha, beta, !is_maximizing, game_state);
        game_state.unmake_move();

        if ((color == piece::Color::WHITE && move_eval > best_eval) ||
            (color == piece::Color::BLACK && move_eval < best_eval)) {
            // std::cout << "Evaluation: " << color << " " << move_eval << " > " << best_eval << std::endl;
            // std::cout << game_state.get_board().to_string() << std::endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(500));
            best_eval = move_eval;
            best_move = move;
        }

        // Update alpha and beta
        if (color == piece::Color::WHITE) {
            alpha = std::max(alpha, best_eval);
        } else {
            beta = std::min(beta, best_eval);
        }

        // Prune if possible
        if (beta <= alpha) {
            break;
        }
    }

    return best_move;
}

} // namespace search
} // namespace chess_engine