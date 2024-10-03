#ifndef CHESS_ENGINE_EVALUATE_H
#define CHESS_ENGINE_EVALUATE_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/game_state.h"
#include "../structure/square.h"

namespace chess_engine {
namespace evaluate {

int evaluate(piece::Color color, game_state::Game_State &state);
int piece_value(piece::Type type);

} // namespace evaluate
} // namespace chess_engine

#endif