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

constexpr bit::Bitboard calculate_all_knight_moves(int sq) {
    bit::Bitboard b = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    const int knight_moves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for (const auto &move : knight_moves) {
        int new_rank = rank + move[0];
        int new_file = file + move[1];
        if (new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8) {
            b |= 1ULL << (new_rank * 8 + new_file);
        }
    }
    return b;
}

struct Knight_Moves {
    std::array<bit::Bitboard, 64> moves;

    constexpr Knight_Moves() : moves() {
        for (int sq = 0; sq < 64; ++sq) {
            moves[sq] = calculate_all_knight_moves(sq);
        }
    }
};

constexpr Knight_Moves knight_moves{};

constexpr bit::Bitboard get_all_knight_moves(int square) {
    return knight_moves.moves[square];
}

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard curr_knights = (color == board::piece::Color::WHITE) ? board.get_white_knights() : board.get_black_knights();

    if (((1ULL << from) & curr_knights) == 0) {
        std::ostringstream oss;
        oss << "Knight at position " << static_cast<int>(from) << " does not exist";
        throw std::invalid_argument(oss.str());
    }

    bit::Bitboard all_knight_moves = get_all_knight_moves(from);
    bit::Bitboard valid_squares = (color == board::piece::Color::WHITE) ? ~board.get_white_pieces() : ~board.get_black_pieces();

    return all_knight_moves & valid_squares;
}

} // namespace knight
} // namespace chess_engine