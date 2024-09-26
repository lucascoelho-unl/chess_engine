#ifndef CHESS_ENGINE_KING_H
#define CHESS_ENGINE_KING_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"

namespace chess_engine {
namespace king {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board);

} // namespace king
} // namespace chess_engine

#endif