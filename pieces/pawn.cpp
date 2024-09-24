#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>
#include <string>

namespace chess_engine {
namespace pawn {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_pawns = (color == board::piece::WHITE) ? board.get_white_pawns() : board.get_black_pawns();
    bit::Bitboard curr_position = ((1ULL & 1) << from) & all_pawns;
    bit::Bitboard valid_positions = board.get_empty_squares();

    if (curr_position == 0) {
        std::ostringstream oss;
        oss << "Pawn at position " << static_cast<int>(from) << " does not exist";
        throw std::invalid_argument(oss.str());
    }

    bit::Bitboard p1 = 0ULL; // move 1 forward
    bit::Bitboard p2 = 0ULL; // move 2 forward
    bit::Bitboard p3 = 0ULL; // capture left
    bit::Bitboard p4 = 0ULL; // capture right

    if (color == board::piece::WHITE) {
        p1 = (curr_position << 8) & valid_positions;
        if ((p1 != 0) && (from >= 8 && from < 16))
            p2 = (curr_position << 16) & valid_positions;
        p3 = ((curr_position) << 7) & board.get_black_pawns();
        p4 = ((curr_position) << 9) & board.get_black_pawns();
    }

    if (color == board::piece::BLACK) {
        p1 = (curr_position >> 8) & valid_positions;
        if ((p1 != 0) && (from >= 48 && from < 56))
            p2 = (curr_position >> 16) & valid_positions;
        p3 = ((curr_position) >> 7) & board.get_black_pawns();
        p4 = ((curr_position) >> 9) & board.get_black_pawns();
    }

    return p1 | p2 | p3 | p4;
}

} // namespace pawn
} // namespace chess_engine