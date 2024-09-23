#ifndef BITBOARD_H
#define BITBOARD_H

#include "square.h"
#include <cstdint>
#include <iostream>

typedef uint64_t Bitboard;

inline Bitboard set_bit(Bitboard b, Square square) {
    return b | (1ULL << square);
}

inline Bitboard clear_bit(Bitboard b, Square square) {
    return b & ~(1ULL << square);
}

inline bool test_bit(Bitboard b, Square square) {
    return (b & (1ULL << square)) != 0;
}

// Using integers to represent position
inline Bitboard set_bit(Bitboard b, int square) {
    return b | (1ULL << square);
}

inline Bitboard clear_bit(Bitboard b, int square) {
    return b & ~(1ULL << square);
}

inline bool test_bit(Bitboard b, int square) {
    return (b & (1ULL << square)) != 0;
}

// Using strings to represent position
inline Bitboard set_bit(Bitboard b, const std::string &s) {
    return set_bit(b, string_to_square(s));
}

inline Bitboard clear_bit(Bitboard b, const std::string &s) {
    return clear_bit(b, string_to_square(s));
}

inline Bitboard test_bit(Bitboard b, const std::string &s) {
    return test_bit(b, string_to_square(s));
}

#endif