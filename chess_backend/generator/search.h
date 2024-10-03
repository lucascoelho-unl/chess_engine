#ifndef CHESS_ENGINE_SEARCH_H
#define CHESS_ENGINE_SEARCH_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/game_state.h"
#include "../structure/square.h"

namespace chess_engine {
namespace search {

moves::Move find_best_move(int depth, piece::Color color, game_state::Game_State &game_state);
int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer, game_state::Game_State &game_state);

} // namespace search
} // namespace chess_engine

#endif