#ifndef CHESS_ENGINE_MOVES_DIAGONAL_H
#define CHESS_ENGINE_MOVES_DIAGONAL_H

#include "../../structure/bitboard.h"
#include "../../structure/board.h"
#include "../../structure/square.h"

namespace chess_engine {
namespace moves {
namespace diagonal {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board);

} // namespace diagonal
} // namespace moves
} // namespace chess_engine

#endif