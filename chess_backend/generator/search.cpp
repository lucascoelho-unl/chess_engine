#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include <algorithm>
#include <iostream>
#include <limits>
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
            score += 150;
            // Prioritize capturing higher value pieces
            score += 10 * evaluate::piece_value(game_state.get_board().get_piece_type(move.to, utils::opposite_color(move.color)));
        }

        // Prioritize promotions
        if (move.move_type == moves::Type::PROMOTION) {
            score += 1000;
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
    if (depth == 0 || game_state.is_game_over()) {
        // Evaluate the board at the leaf node
        return evaluate::evaluate(game_state);
    }

    if (isMaximizingPlayer) {
        int maxEval = NEG_INF;
        std::vector<moves::Move> possible_moves = order_moves(moves::generate_legal_moves(piece::Color::WHITE, game_state), game_state);

        for (const auto &move : possible_moves) {
            // std::cout << "max    " << moves::to_string(move) << std::endl;
            game_state.make_move(move);
            int eval = minimax(depth - 1, alpha, beta, false, game_state);
            game_state.unmake_move(); // Undo the move
            maxEval = std::max(maxEval, eval);

            // Alpha-Beta Pruning
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return maxEval;

    } else {
        int minEval = INF;
        std::vector<moves::Move> possible_moves = order_moves(moves::generate_legal_moves(piece::Color::BLACK, game_state), game_state);

        for (const auto &move : possible_moves) {
            // std::cout << "min    " << moves::to_string(move) << std::endl;
            game_state.make_move(move);
            int eval = minimax(depth - 1, alpha, beta, true, game_state);
            game_state.unmake_move(); // Undo the move
            minEval = std::min(minEval, eval);

            // Alpha-Beta Pruning
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

moves::Move find_best_move(int depth, piece::Color color, game_state::Game_State &game_state) {
    int best_eval = (color == piece::Color::WHITE) ? NEG_INF : INF;
    moves::Move best_move;

    std::vector<moves::Move> possible_moves = order_moves(moves::generate_legal_moves(color, game_state), game_state);
    bool is_maximizing = (color == piece::Color::WHITE);

    for (const auto &move : possible_moves) {
        game_state.make_move(move);
        int move_eval = minimax(depth - 1, NEG_INF, INF, !is_maximizing, game_state);
        game_state.unmake_move();

        if ((color == piece::Color::WHITE && move_eval > best_eval) ||
            (color == piece::Color::BLACK && move_eval < best_eval)) {
            best_eval = move_eval;
            best_move = move;
        }
    }

    return best_move;
}

} // namespace search
} // namespace chess_engine