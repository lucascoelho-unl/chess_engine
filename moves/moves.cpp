#include "moves.h"
#include "../pieces/pawn.cpp"
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

} // namespace moves
} // namespace chess_engine