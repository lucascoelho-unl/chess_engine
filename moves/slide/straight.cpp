#include "straight.h"
#include "../../structure/bitboard.h"
#include "../../structure/board.h"
#include "../../structure/square.h"
#include "../../utils.h"
#include "../moves.h"
#include <array>
#include <iostream>
#include <vector>

namespace chess_engine {
namespace moves {
namespace straight {

bit::Bitboard calculate_straight_moves_for_square(int sq) {
    bit::Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // Directions: North, East, South, West
    int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    for (int d = 0; d < 4; ++d) {
        int r = rank + directions[d][0];
        int f = file + directions[d][1];

        if (directions[d][0] != 0) {
            while (r >= 1 && r <= 6) {
                attacks |= (1ULL << (r * 8 + f));
                r += directions[d][0];
            }
        } else {
            while (f >= 1 && f <= 6) {
                attacks |= (1ULL << (r * 8 + f));
                f += directions[d][1];
            }
        }
    }
    return attacks;
}

std::vector<bit::Bitboard> generate_straight_moves() {
    std::vector<bit::Bitboard> straight_moves(64);
    for (int sq = 0; sq < 64; ++sq) {
        straight_moves[sq] = calculate_straight_moves_for_square(sq);
    }
    return straight_moves;
}

std::vector<bit::Bitboard> straight_moves = generate_straight_moves();

bit::Bitboard compute_attack_for_occupancy(int sq, bit::Bitboard occupancy) {
    bit::Bitboard atk = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // North (Up the board)
    for (int i = rank + 1; i < 8; ++i) {
        int target_square = (i * 8) + file;
        atk |= (1ULL << target_square);
        // If there is a piece on this square, stop
        if (occupancy & (1ULL << target_square))
            break;
    }

    // South (Down the board)
    for (int i = rank - 1; i >= 0; --i) {
        int target_square = (i * 8) + file;
        atk |= (1ULL << target_square);
        // If there is a piece on this square, stop
        if (occupancy & (1ULL << target_square))
            break;
    }

    // East (Right across the board)
    for (int i = file + 1; i < 8; ++i) {
        int target_square = (rank * 8) + i;
        atk |= (1ULL << target_square);
        // If there is a piece on this square, stop
        if (occupancy & (1ULL << target_square))
            break;
    }

    // West (Left across the board)
    for (int i = file - 1; i >= 0; --i) {
        int target_square = (rank * 8) + i;
        atk |= (1ULL << target_square);
        // If there is a piece on this square, stop
        if (occupancy & (1ULL << target_square))
            break;
    }

    return atk;
}

std::vector<bit::Bitboard> get_occupancy_variations(int square) {
    bit::Bitboard atk_mask = straight_moves[square];
    int bits_in_mask = __builtin_popcountll(atk_mask);
    int occupancy_count = 1 << bits_in_mask; // 2^bitsInMask

    std::vector<bit::Bitboard> occupancies(occupancy_count); // Pre-allocate size for all attack variations
    std::vector<int> mask_bits_indices(bits_in_mask);        // Pre-allocate based on the number of blockers
    int index = 0;

    // Populate mask_bits_indices
    for (int i = 0; i < 64; ++i) {
        if (atk_mask & (1ULL << i)) {
            mask_bits_indices[index++] = i;
        }
    }

    // Generate all possible combinations of attacks
    for (int i = 0; i < occupancy_count; ++i) {
        bit::Bitboard occupancy = 0ULL;
        for (int j = 0; j < bits_in_mask; ++j) {
            if (i & (1 << j)) {
                occupancy |= (1ULL << mask_bits_indices[j]);
            }
        }
        occupancies[i] = occupancy;
    }
    return occupancies;
}

// clang-format off
std::vector<int> shift_values = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12,
};

std::vector<bit::Bitboard> magic_numbers = {
	0x11800040001481a0ULL,
	0x2040400010002000ULL,
	0xa280200308801000ULL,
	0x100082005021000ULL,
	0x280280080040006ULL,
	0x200080104100200ULL,
	0xc00040221100088ULL,
	0xe00072200408c01ULL,
	0x2002045008600ULL,
	0xa410804000200089ULL,
	0x4081002000401102ULL,
	0x2000c20420010ULL,
	0x800800400080080ULL,
	0x40060010041a0009ULL,
	0x441004442000100ULL,
	0x462800080004900ULL,
	0x80004020004001ULL,
	0x1840420021021081ULL,
	0x8020004010004800ULL,
	0x940220008420010ULL,
	0x2210808008000400ULL,
	0x24808002000400ULL,
	0x803604001019a802ULL,
	0x520000440081ULL,
	0x802080004000ULL,
	0x1200810500400024ULL,
	0x8000100080802000ULL,
	0x2008080080100480ULL,
	0x8000404002040ULL,
	0xc012040801104020ULL,
	0xc015000900240200ULL,
	0x20040200208041ULL,
	0x1080004000802080ULL,
	0x400081002110ULL,
	0x30002000808010ULL,
	0x2000100080800800ULL,
	0x2c0800400800800ULL,
	0x1004800400800200ULL,
	0x818804000210ULL,
	0x340082000a45ULL,
	0x8520400020818000ULL,
	0x2008900460020ULL,
	0x100020008080ULL,
	0x601001000a30009ULL,
	0xc001000408010010ULL,
	0x2040002008080ULL,
	0x11008218018c0030ULL,
	0x20c0080620011ULL,
	0x400080002080ULL,
	0x8810040002500ULL,
	0x400801000200080ULL,
	0x2402801000080480ULL,
	0x204040280080080ULL,
	0x31044090200801ULL,
	0x40c10830020400ULL,
	0x442800100004080ULL,
	0x10080002d005041ULL,
	0x134302820010a2c2ULL,
	0x6202001080200842ULL,
	0x1820041000210009ULL,
	0x1002001008210402ULL,
	0x2000108100402ULL,
	0x10310090a00b824ULL,
	0x800040100944822ULL,
};
// clang-format on

int magic_hash(bit::Bitboard occupancy, bit::Bitboard magic_number, int shift) {
    return ((occupancy * magic_number) >> (64 - shift));
}

std::vector<std::vector<bit::Bitboard>> precompute_attack_table() {
    std::vector<std::vector<bit::Bitboard>> attack_table(64);
    for (int square = 0; square < 64; ++square) {
        bit::Bitboard magic_number = magic_numbers[square];               // Get the magic number for the square
        int shift = shift_values[square];                                 // Get the shift value for the square
        int relevant_bits = __builtin_popcountll(straight_moves[square]); // Number of relevant bits for this square

        // Calculate the number of possible occupancy configurations
        int occupancy_count = 1 << relevant_bits;

        // Resize the attack table for this square
        attack_table[square].resize(occupancy_count);

        std::vector<bit::Bitboard> occupancy_variations = get_occupancy_variations(square);
        for (bit::Bitboard occ : occupancy_variations) {
            bit::Bitboard attack = compute_attack_for_occupancy(square, occ);
            int index = magic_hash(occ, magic_number, shift);
            attack_table[square][index] = attack;
        }
    }
    return attack_table;
};

std::vector<std::vector<bit::Bitboard>> attack_table = precompute_attack_table();

bit::Bitboard get_moves(square::Square from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard relevant_occupancy = board.get_occupied_squares() & straight_moves[from];
    int index = magic_hash(relevant_occupancy, magic_numbers[from], shift_values[from]);
    return attack_table[from][index];
}

} // namespace straight
} // namespace moves
} // namespace chess_engine