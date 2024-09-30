#include "square.h"
#include <stdexcept>
#include <string>
#include <utility>

namespace chess_engine {
namespace square {

Square int_to_square(int position) {
    if (position < 0 || position > 63) {
        throw std::out_of_range("Invalid square position");
    }
    return static_cast<Square>(position);
}

Square file_rank_to_square(int file, int rank) {
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        throw std::out_of_range("Invalid file or rank");
    }
    return static_cast<Square>(rank * 8 + file);
}

std::pair<int, int> square_to_file_rank(Square square) {
    int file = square % 8;
    int rank = square / 8;
    return {file, rank};
}

std::string square_to_string(Square square) {
    const char files[] = "abcdefgh";
    const char ranks[] = "12345678";
    int file = square % 8;
    int rank = square / 8;
    return std::string(1, files[file]) + ranks[rank];
}

Square string_to_square(const std::string &square_str) {
    if (square_str.length() != 2) {
        throw std::invalid_argument("Invalid square string length");
    }

    const char file = square_str[0];
    const char rank = square_str[1];

    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        throw std::invalid_argument("Invalid file or rank");
    }

    int file_idx = file - 'a';
    int rank_idx = rank - '1';

    return file_rank_to_square(file_idx, rank_idx);
}

std::string int_position_to_string(int square) {
    char file = 'a' + (square % 8); // Calculate file (a-h)
    char rank = '1' + (square / 8); // Calculate rank (1-8)
    return std::string(1, file) + rank;
}

} // namespace square
} // namespace chess_engine
