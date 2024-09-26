#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include "../utils.h"
#include <array>
#include <cstdint>
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace knight {

// Precompute knight moves for each square on the board
constexpr std::array<bit::Bitboard, 64> calculate_knight_moves() {
    std::array<bit::Bitboard, 64> moves = {0ULL};

    const int knight_moves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    for (int sq = 0; sq < 64; ++sq) {
        bit::Bitboard b = 0ULL;
        int rank = sq / 8;
        int file = sq % 8;

        for (const auto &move : knight_moves) {
            int new_rank = rank + move[0];
            int new_file = file + move[1];
            if (new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8) {
                b |= 1ULL << (new_rank * 8 + new_file);
            }
        }
        moves[sq] = b;
    }

    return moves;
}

// Precompute knight moves once at compile-time
constexpr std::array<bit::Bitboard, 64> knight_moves = calculate_knight_moves();

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard curr_knights = board.get_knights(color);

    if (((1ULL << from) & curr_knights) == 0) {
        return 0ULL;
    }

    bit::Bitboard all_knight_moves = knight_moves[static_cast<int>(from)];
    bit::Bitboard valid_squares = (color == board::piece::Color::WHITE) ? ~board.get_white_pieces() : ~board.get_black_pieces();

    return all_knight_moves & valid_squares;
}

} // namespace knight
} // namespace chess_engine
