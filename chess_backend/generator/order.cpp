#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include <algorithm>
#include <vector>

namespace chess_engine {
namespace order {

std::vector<moves::Move> order_moves(const std::vector<moves::Move> &moves, const game_state::GameState &game_state) {
    std::vector<std::pair<int, moves::Move>> scored_moves;
    scored_moves.reserve(moves.size());

    // Example piece-square table for pawns (can extend this for other pieces)
    int pawn_table[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 10, 10, -20, -20, 10, 10, 5,
        5, -5, -10, 0, 0, -10, -5, 5,
        0, 0, 0, 20, 20, 0, 0, 0,
        5, 5, 10, 25, 25, 10, 5, 5,
        10, 10, 20, 30, 30, 20, 10, 10,
        50, 50, 50, 50, 50, 50, 50, 50,
        0, 0, 0, 0, 0, 0, 0, 0};

    for (const auto &move : moves) {
        int score = 0;

        // Prioritize captures
        if (move.move_type == moves::Type::CAPTURE) {
            score += 9000;
            score += 100 * evaluate::piece_value(game_state.get_board().get_piece_type(move.to, utils::opposite_color(move.color)));
        }

        // Prioritize promotions
        if (move.move_type == moves::Type::PROMOTION) {
            score += 10000;
        }

        // Encourage central control (dynamic central control)
        if (move.to >= 27 && move.to <= 36) { // e4, e5, d4, d5
            score += 500;
        }

        // Piece-square table bonus for pawns (extend this to other pieces)
        if (game_state.get_board().get_piece_type(move.from, move.color) == piece::Type::PAWN) {
            score += pawn_table[move.to];
        }

        // Encourage knight and bishop development
        if (move.move_type == moves::Type::NORMAL) {
            if (move.to == 18 || move.to == 21 || move.to == 42 || move.to == 45) { // c3, f3, c6, f6
                score += 400;                                                       // Encourage knight and bishop centralization
            }
        }

        // Castling bonuses (encourage king safety)
        if (move.move_type == moves::Type::CASTLING) {
            score += 1000; // Encourage castling for king safety
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

} // namespace order
} // namespace chess_engine