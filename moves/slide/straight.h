#ifndef CHESS_ENGINE_MOVES_STRAIGHT_H
#define CHESS_ENGINE_MOVES_STRAIGHT_H

#include "../../structure/bitboard.h"
#include "../../structure/board.h"
#include "../../structure/square.h"

namespace chess_engine {
namespace moves {
namespace straight {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board);

} // namespace straight
} // namespace moves
} // namespace chess_engine

#endif