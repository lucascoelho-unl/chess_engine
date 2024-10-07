#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "evaluate.h"
#include <algorithm>
#include <vector>

namespace chess_engine {
namespace order {

constexpr int MVV_LVA[6][6] = {
    {15, 14, 13, 12, 11, 10}, // Victim Pawn
    {25, 24, 23, 22, 21, 20}, // Victim Knight
    {35, 34, 33, 32, 31, 30}, // Victim Bishop
    {45, 44, 43, 42, 41, 40}, // Victim Rook
    {55, 54, 53, 52, 51, 50}, // Victim Queen
    {0, 0, 0, 0, 0, 0}        // Victim King
};

std::vector<moves::Move> order_moves(const std::vector<moves::Move> &moves, const game_state::GameState &game_state) {
    std::vector<std::pair<int, moves::Move>> scored_moves;
    scored_moves.reserve(moves.size());

    for (const auto &move : moves) {
        int score = 0;

        // Prioritize captures using MVV-LVA
        if (move.move_type == moves::Type::CAPTURE) {
            piece::Type victim = game_state.get_board().get_piece_type(move.to);
            piece::Type attacker = move.piece_type;
            score += MVV_LVA[victim][attacker];
        }

        // Prioritize promotions
        if (move.move_type == moves::Type::PROMOTION) {
            score += 2000 + static_cast<int>(move.promotion);
        }

        // Encourage central pawn moves in the opening
        if (move.piece_type == piece::Type::PAWN && game_state.fullmove_number <= 10) {
            int from_file = move.from % 8;
            int to_file = move.to % 8;
            if ((from_file == 3 || from_file == 4) && (to_file == 3 || to_file == 4)) {
                score += 50;
            }
        }

        // Encourage knight and bishop development in the opening
        if ((move.piece_type == piece::Type::KNIGHT || move.piece_type == piece::Type::BISHOP) &&
            game_state.fullmove_number <= 10) {
            int from_rank = move.from / 8;
            int to_rank = move.to / 8;
            if ((from_rank == 0 || from_rank == 7) && (to_rank != 0 && to_rank != 7)) {
                score += 30;
            }
        }

        // Encourage castling
        if (move.move_type == moves::Type::CASTLING) {
            score += 60;
        }

        // Penalize moving the same piece twice in the opening
        // if (game_state.get_fullmove_number() <= 10) {
        // This would require keeping track of piece move counts in the GameState
        // For simplicity, omit this for now
        // }

        // Use history heuristic
        // This would require maintaining a history table
        // score += history_table[move.from][move.to];

        // Killer move heuristic
        // This would require maintaining killer moves for each ply
        // if (is_killer_move(move, ply)) score += 50;

        scored_moves.emplace_back(score, move);
    }

    // Sort moves by score in descending order
    std::sort(scored_moves.begin(), scored_moves.end(), std::greater<>());

    std::vector<moves::Move> ordered_moves;
    ordered_moves.reserve(moves.size());
    for (const auto &scored_move : scored_moves) {
        ordered_moves.push_back(scored_move.second);
    }

    return ordered_moves;
}

} // namespace order
} // namespace chess_engine