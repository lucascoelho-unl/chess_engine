#ifndef CHESS_ENGINE_BISHOP_H
#define CHESS_ENGINE_BISHOP_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"

namespace chess_engine {
namespace bishop {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board);

} // namespace bishop
} // namespace chess_engine

#endif