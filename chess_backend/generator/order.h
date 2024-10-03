#ifndef CHESS_ENGINE_ORDER_H
#define CHESS_ENGINE_ORDER_H

#include "../enums.h"
#include "../structure/game_state.h"

namespace chess_engine {
namespace order {

std::vector<moves::Move> order_moves(const std::vector<moves::Move> &moves, const game_state::Game_State &game_state);

} // namespace order
} // namespace chess_engine

#endif