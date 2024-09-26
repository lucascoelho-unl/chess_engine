#include "../moves/slide/straight.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace rook {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard rook_positions = board.get_rooks(color);
    if (((1ULL << from) & rook_positions) == 0) {
        return 0ULL;
    }

    bit::Bitboard rook_moves = moves::straight::get_moves(from, color, board);
    bit::Bitboard own_pieces = (color == board::piece::Color::WHITE) ? board.get_white_pieces() : board.get_black_pieces();

    return rook_moves & ~own_pieces;
}

} // namespace rook
} // namespace chess_engine