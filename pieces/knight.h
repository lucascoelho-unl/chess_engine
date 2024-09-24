#ifndef CHESS_ENGINE_KNIGHT_H
#define CHESS_ENGINE_KNIGHT_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"

namespace chess_engine {
namespace knight {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board);

} // namespace knight
} // namespace chess_engine

#endif