#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>
#include <iostream>
#include <map>
#include <string>

typedef uint64_t Bitboard;

inline Bitboard set_bit(Bitboard b, int square) {
    return b | (1ULL << square);
}

inline Bitboard clear_bit(Bitboard b, int square) {
    return b & ~(1ULL << square);
}

inline bool test_bit(Bitboard b, int square) {
    return (b & (1ULL << square)) != 0;
}

inline int chess_notation_to_square(const std::string &s) {
    if (s.length() != 2) {
        return -1;
    }

    std::map<char, int> map = {
        {'a', 0},
        {'b', 1},
        {'c', 2},
        {'d', 3},
        {'e', 4},
        {'f', 5},
        {'g', 6},
        {'h', 7},
    };

    char rank = s[0];
    if (map.find(rank) == map.end()) {
        return -1;
    }

    int file = (s[1] - '1') * 8;
    if (file < 0 || file > 64) {
        return -1;
    }

    return map[rank] + file;
}

inline Bitboard set_bit(Bitboard b, std::string s) {
    return b | (1ULL << chess_notation_to_square(s));
}

inline Bitboard clear_bit(Bitboard b, std::string s) {
    return b & ~(1ULL << chess_notation_to_square(s));
}

#endif