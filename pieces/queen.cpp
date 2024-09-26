#include "../moves/slide/diagonal.h"
#include "../moves/slide/straight.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include "../utils.h"
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace queen {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard queen_position = board.get_queens(color);
    if (((1ULL << from) & queen_position) == 0) {
        std::ostringstream oss;
        oss << "Queen at position " << static_cast<int>(from) << " does not exist";
        throw std::invalid_argument(oss.str());
    }

    bit::Bitboard straight_queen_moves = moves::straight::get_moves(from, color, board);
    bit::Bitboard diagonal_queen_moves = moves::diagonal::get_moves(from, color, board);
    bit::Bitboard own_pieces = (color == board::piece::Color::WHITE) ? board.get_white_pieces() : board.get_black_pieces();

    return (straight_queen_moves | diagonal_queen_moves) & ~own_pieces;
}

} // namespace queen
} // namespace chess_engine