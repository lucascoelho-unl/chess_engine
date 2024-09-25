#include "diagonal.h"
#include "../../structure/bitboard.h"
#include "../../structure/board.h"
#include "../../structure/square.h"
#include "../../utils.h"
#include <array>

namespace chess_engine {
namespace moves {
namespace diagonal {

constexpr bit::Bitboard calculate_all_diagonal_moves(int sq) {
    bit::Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // Directions: North-East, North-West, South-East, South-West
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (int d = 0; d < 4; ++d) {
        int r = rank + directions[d][0];
        int f = file + directions[d][1];

        while (r >= 1 && r <= 6 && f >= 1 && f <= 6) {
            attacks |= (1ULL << (r * 8 + f));
            r += directions[d][0];
            f += directions[d][1];
        }
    }
    return attacks;
}

std::array<bit::Bitboard, 64> moves;

constexpr std::array<bit::Bitboard, 64> generate_diagonal_moves() {
    std::array<bit::Bitboard, 64> diagonal_moves{};
    for (int sq = 0; sq < 64; ++sq) {
        diagonal_moves[sq] = calculate_all_diagonal_moves(sq);
    }
    return diagonal_moves;
}

constexpr std::array<bit::Bitboard, 64> diagonal_moves = generate_diagonal_moves();

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    for (bit::Bitboard move : diagonal_moves) {
        utils::print_bitboard(move);
    }
    return 1ULL;
}

} // namespace diagonal
} // namespace moves
} // namespace chess_engine