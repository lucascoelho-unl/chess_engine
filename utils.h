#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

namespace utils {

using namespace chess_engine;

void print_bitboard(bit::Bitboard b);

} // namespace utils
#endif