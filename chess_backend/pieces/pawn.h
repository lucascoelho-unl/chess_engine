#ifndef CHESS_ENGINE_PAWN_H
#define CHESS_ENGINE_PAWN_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/game_state.h"
#include "../structure/square.h"

namespace chess_engine {
namespace pawn {

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state);

bit::Bitboard get_possible_attacks(piece::Color color, bit::Bitboard pawns);

} // namespace pawn
} // namespace chess_engine

#endif // CHESS_ENGINE_PAWN_H