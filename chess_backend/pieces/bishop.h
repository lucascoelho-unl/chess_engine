#ifndef CHESS_ENGINE_BISHOP_H
#define CHESS_ENGINE_BISHOP_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/game_state.h"
#include "../structure/square.h"

namespace chess_engine {
namespace bishop {

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state);

} // namespace bishop
} // namespace chess_engine

#endif