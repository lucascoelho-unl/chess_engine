#ifndef CHESS_ENGINE_SQUARE_H
#define CHESS_ENGINE_SQUARE_H

#include "../enums.h"
#include <stdexcept>
#include <string>
#include <utility>

namespace chess_engine {
namespace square {

Square int_to_square(int position);

Square file_rank_to_square(int file, int rank);

std::pair<int, int> square_to_file_rank(Square square);

std::string square_to_string(Square square);

Square string_to_square(const std::string &square_str);

} // namespace square
} // namespace chess_engine
#endif
