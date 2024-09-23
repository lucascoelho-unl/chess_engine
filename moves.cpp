#include "moves.h"
#include "bitboard.h"
#include "board.h"
#include "pieces/pawn.cpp"
#include "square.h"
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