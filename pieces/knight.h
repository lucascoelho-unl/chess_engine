#ifndef CHESS_ENGINE_KNIGHT_H
#define CHESS_ENGINE_KNIGHT_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/game_state.h"
#include "../structure/square.h"

namespace chess_engine {
namespace knight {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

} // namespace knight
} // namespace chess_engine

#endif