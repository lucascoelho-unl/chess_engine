#ifndef CHESS_ENGINE_UTILS_H
#define CHESS_ENGINE_UTILS_H

#include "enums.h"
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

piece::Color opposite_color(piece::Color c);

std::string move_type_to_string(moves::Type type);

std::string piece_type_to_string(piece::Type type);

std::string piece_color_to_string(piece::Color color);

} // namespace utils
#endif