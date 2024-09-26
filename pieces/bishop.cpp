#include "../moves/slide/diagonal.h"
#include "../structure/bitboard.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace bishop {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard bishop_positions = board.get_bishops(color);
    if (((1ULL << from) & bishop_positions) == 0) {
        std::ostringstream oss;
        oss << "Bishop at position " << static_cast<int>(from) << " does not exist";
        throw std::invalid_argument(oss.str());
    }

    bit::Bitboard bishop_moves = moves::diagonal::get_moves(from, color, board);
    bit::Bitboard own_pieces = (color == board::piece::Color::WHITE) ? board.get_white_pieces() : board.get_black_pieces();

    return bishop_moves & ~own_pieces;
}

} // namespace bishop
} // namespace chess_engine