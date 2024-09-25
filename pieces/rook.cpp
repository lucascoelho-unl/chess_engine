#include "../moves/slide/straight.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"

namespace chess_engine {
namespace rook {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard rook_moves = moves::straight::get_moves(from, color, board);
    bit::Bitboard own_pieces = (color == board::piece::Color::WHITE) ? board.get_white_pieces() : board.get_black_pieces();

    return rook_moves & ~own_pieces;
}

} // namespace rook
} // namespace chess_engine