#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include "../utils.h"
#include <array>
#include <sstream>
#include <stdexcept>

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

// Precompute the king moves for all 64 squares
constexpr std::array<bit::Bitboard, 64> king_moves = calculate_king_moves();

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard curr_king = board.get_king(color);

    if (((1ULL << from) & curr_king) == 0) {
        return 0ULL;
    }

    bit::Bitboard available_king_moves = king_moves[static_cast<int>(from)];
    bit::Bitboard valid_squares = (color == board::piece::Color::WHITE) ? ~board.get_white_pieces() : ~board.get_black_pieces();
    return available_king_moves & valid_squares;
}

} // namespace king
} // namespace chess_engine
