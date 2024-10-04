#include "../enums.h"
#include "../moves/slide/diagonal.h"
#include "../moves/slide/straight.h"
#include "../structure/bitboard.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "../utils.h"
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace queen {

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state) {
    bit::Bitboard queen_position = board.get_queens(color);
    if (((1ULL << from) & queen_position) == 0) {
        return 0ULL;
    }

    bit::Bitboard straight_queen_moves = moves::straight::get_moves(from, color, board);
    bit::Bitboard diagonal_queen_moves = moves::diagonal::get_moves(from, color, board);
    bit::Bitboard own_pieces = (color == piece::Color::WHITE) ? board.get_white_pieces() : board.get_black_pieces();

    return (straight_queen_moves | diagonal_queen_moves) & ~own_pieces;
}

} // namespace queen
} // namespace chess_engine