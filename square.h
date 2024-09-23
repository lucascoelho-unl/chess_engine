#ifndef SQUARE_H
#define SQUARE_H

#include <stdexcept>
#include <string>
#include <utility>

// clang-format off
enum Square {
    A1 = 0,  B1,  C1,  D1,  E1,  F1,  G1,  H1,
    A2,  B2,  C2,  D2,  E2,  F2,  G2,  H2,
    A3,  B3,  C3,  D3,  E3,  F3,  G3,  H3,
    A4,  B4,  C4,  D4,  E4,  F4,  G4,  H4,
    A5,  B5,  C5,  D5,  E5,  F5,  G5,  H5,
    A6,  B6,  C6,  D6,  E6,  F6,  G6,  H6,
    A7,  B7,  C7,  D7,  E7,  F7,  G7,  H7,
    A8,  B8,  C8,  D8,  E8,  F8,  G8,  H8,
};
// clang-format on

inline Square int_to_square(int position) {
    if (position < 0 || position > 63) {
        throw std::out_of_range("Invalid square position");
    }
    return static_cast<Square>(position);
}

inline Square file_rank_to_square(int file, int rank) {
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        throw std::out_of_range("Invalid file or rank");
    }
    return static_cast<Square>(rank * 8 + file);
}

inline std::pair<int, int> square_to_file_rank(Square square) {
    int file = square % 8;
    int rank = square / 8;
    return {file, rank};
}

inline std::string square_to_string(Square square) {
    const char files[] = "abcdefgh";
    const char ranks[] = "12345678";
    int file = square % 8;
    int rank = square / 8;
    return std::string(1, files[file]) + ranks[rank];
}

inline Square string_to_square(const std::string &square_str) {
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

#endif
