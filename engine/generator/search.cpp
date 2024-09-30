#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include <chrono> // For time units
#include <iostream>
#include <limits>
#include <thread> // For sleep_for

namespace chess_engine {
namespace search {

const int INF = std::numeric_limits<int>::max();
const int NEG_INF = std::numeric_limits<int>::min();

int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer, game_state::Game_State &game_state) {
    if (depth == 0 || game_state.is_game_over()) {
        // Evaluate the board at the leaf node
        return evaluate::evaluate(game_state);
    }

    if (isMaximizingPlayer) {
        int maxEval = NEG_INF;
        std::vector<moves::Move> possibleMoves = moves::generate_legal_moves(piece::Color::WHITE, game_state);

        for (const auto &move : possibleMoves) {
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
        std::vector<moves::Move> possibleMoves = moves::generate_legal_moves(piece::Color::BLACK, game_state);

        for (const auto &move : possibleMoves) {
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

    std::vector<moves::Move> possibleMoves = moves::generate_legal_moves(color, game_state);
    bool is_maximizing = (color == piece::Color::WHITE);

    for (const auto &move : possibleMoves) {
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