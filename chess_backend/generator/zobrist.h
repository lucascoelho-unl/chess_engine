#ifndef CHESS_ENGINE_ZOBRIST_H
#define CHESS_ENGINE_ZOBRIST_H

#include "../enums.h"
#include <array>
#include <cstdint>
#include "../structure/game_state.h"

namespace chess_engine {
namespace zobrist {

extern std::array<std::array<uint64_t, 64>, 12> piece_keys;
extern std::array<uint64_t, 16> castling_keys;
extern std::array<uint64_t, 8> en_passant_keys;
extern uint64_t side_to_move_key;

void init_zobrist_keys();
uint64_t compute_hash(const game_state::GameState &state);

} // namespace zobrist
} // namespace chess_engine

#endif