#include "moves.h"
#include "bitboard.h"
#include "board.h"
#include "square.h"
#include <sstream>
#include <stdexcept>
#include <string>

Bitboard get_pawn_moves(Square from, Color color, const Board &board) {
    Bitboard all_pawns = (color == WHITE) ? board.get_white_pawns() : board.get_black_pawns();
    Bitboard curr_position = ((1ULL & 1) << from) & all_pawns;
    Bitboard valid_positions = board.get_empty_squares();

    if (curr_position == 0) {
        std::ostringstream oss;
        oss << "Pawn at position " << static_cast<int>(from) << " does not exist";
        throw std::invalid_argument(oss.str());
    }

    Bitboard p1 = 0ULL; // move 1 forward
    Bitboard p2 = 0ULL; // move 2 forward
    Bitboard p3 = 0ULL; // capture left
    Bitboard p4 = 0ULL; // capture right

    if (color == WHITE) {
        p1 = (curr_position << 8) & valid_positions;
        if ((p1 != 0) && (8 <= from <= 15))
            p2 = (curr_position << 16) & valid_positions;
        p3 = ((curr_position) << 7) & board.get_black_pawns();
        p4 = ((curr_position) << 9) & board.get_black_pawns();
    }

    if (color == BLACK) {
        p1 = (curr_position >> 8) & valid_positions;
        if ((p1 != 0) && (48 <= from <= 55))
            p2 = (curr_position >> 16) & valid_positions;
        p3 = ((curr_position) >> 7) & board.get_black_pawns();
        p4 = ((curr_position) >> 9) & board.get_black_pawns();
    }

    return p1 | p2 | p3 | p4;
}