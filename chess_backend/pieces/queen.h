#ifndef CHESS_ENGINE_QUEEN_H
#define CHESS_ENGINE_QUEEN_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"

namespace chess_engine {
namespace queen {

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state);

} // namespace queen
} // namespace chess_engine

#endif