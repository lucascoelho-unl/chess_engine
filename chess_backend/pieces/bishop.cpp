#include "../enums.h"
#include "../moves/slide/diagonal.h"
#include "../structure/bitboard.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace bishop {

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state) {
    bit::Bitboard bishop_positions = board.get_bishops(color);
    if (((1ULL << from) & bishop_positions) == 0) {
        return 0ULL;
    }

    bit::Bitboard bishop_moves = moves::diagonal::get_moves(from, color, board);
    bit::Bitboard own_pieces = (color == piece::Color::WHITE) ? board.get_white_pieces() : board.get_black_pieces();

    return bishop_moves & ~own_pieces;
}

} // namespace bishop
} // namespace chess_engine