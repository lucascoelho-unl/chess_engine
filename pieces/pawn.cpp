#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>
#include <string>

namespace chess_engine {
namespace pawn {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard curr_position = 1ULL << from;
    bit::Bitboard valid_positions = board.get_empty_squares();

    // Check if pawn exists in this position
    if (!(curr_position & board.get_pawns(color))) {
        return 0ULL;
    }

    bit::Bitboard p1 = 0ULL; // move 1 forward
    bit::Bitboard p2 = 0ULL; // move 2 forward
    bit::Bitboard p3 = 0ULL; // capture left
    bit::Bitboard p4 = 0ULL; // capture right

    if (color == board::piece::WHITE) {
        p1 = (curr_position << 8) & valid_positions;
        if ((p1 != 0) && (from >= 8 && from < 16)) {
            p2 = (curr_position << 16) & valid_positions;
        }
        p3 = ((curr_position & ~board::a_file) << 7) & board.get_black_pieces();
        p4 = ((curr_position & ~board::h_file) << 9) & board.get_black_pieces();
    } else {
        p1 = (curr_position >> 8) & valid_positions;
        if ((p1 != 0) && (from >= 48 && from < 56)) {
            p2 = (curr_position >> 16) & valid_positions;
        }
        p3 = ((curr_position & ~board::h_file) >> 7) & board.get_white_pieces();
        p4 = ((curr_position & ~board::a_file) >> 9) & board.get_white_pieces();
    }

    return p1 | p2 | p3 | p4;
}

} // namespace pawn
} // namespace chess_engine