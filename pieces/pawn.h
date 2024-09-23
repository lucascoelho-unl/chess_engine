// pawn.h
#ifndef CHESS_ENGINE_PAWN_H
#define CHESS_ENGINE_PAWN_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"

namespace chess_engine {
namespace pawn {

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board);

} // namespace pawn
} // namespace chess_engine

#endif // CHESS_ENGINE_PAWN_H