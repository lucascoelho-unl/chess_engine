#ifndef CHESS_ENGINE_KING_H
#define CHESS_ENGINE_KING_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/game_state.h"
#include "../structure/square.h"

namespace chess_engine {
namespace king {

constexpr std::array<bit::Bitboard, 64> calculate_king_moves() {
    std::array<bit::Bitboard, 64> moves = {0ULL}; // Initialize all moves to zero
    const std::array<int, 8> directions = {+1, -1, -7, -8, -9, +7, +8, +9};

    for (int sq = 0; sq < 64; ++sq) {
        bit::Bitboard b = 0ULL;
        int file = sq % 8;

        for (int direction : directions) {
            int move = sq + direction;

            // Check if move stays within the board
            if (move < 0 || move >= 64)
                continue;

            // Handle left edge (file 0) and right edge (file 7) conditions
            int new_file = move % 8;
            if ((file == 0 && (direction == -1 || direction == -9 || direction == +7)) ||
                (file == 7 && (direction == +1 || direction == +9 || direction == -7))) {
                continue;
            }

            // Add valid move to the bitboard
            b |= 1ULL << move;
        }
        moves[sq] = b;
    }

    return moves;
}

constexpr std::array<bit::Bitboard, 64> king_moves = calculate_king_moves();

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state);

} // namespace king
} // namespace chess_engine

#endif