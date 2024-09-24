#include "moves.h"
#include "../pieces/knight.h"
#include "../pieces/pawn.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>
#include <string>

namespace chess_engine {
namespace moves {

bit::Bitboard get_pawn_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    return pawn::get_moves(from, color, board);
}

bit::Bitboard get_knight_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    return knight::get_moves(from, color, board);
}

} // namespace moves
} // namespace chess_engine