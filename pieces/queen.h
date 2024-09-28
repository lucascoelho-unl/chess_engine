#ifndef CHESS_ENGINE_QUEEN_H
#define CHESS_ENGINE_QUEEN_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"

namespace chess_engine {
namespace queen {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

} // namespace queen
} // namespace chess_engine

#endif