#include "diagonal.h"
#include "../../structure/bitboard.h"
#include "../../structure/board.h"
#include "../../structure/square.h"
#include "../../utils.h"
#include <vector>

namespace chess_engine {
namespace moves {
namespace diagonal {

bit::Bitboard calculate_all_diagonal_moves(int sq) {
    bit::Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // Directions: North-East, North-West, South-East, South-West
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (int d = 0; d < 4; ++d) {
        int r = rank + directions[d][0];
        int f = file + directions[d][1];

        while (r >= 1 && r <= 6 && f >= 1 && f <= 6) {
            attacks |= (1ULL << (r * 8 + f));
            r += directions[d][0];
            f += directions[d][1];
        }
    }
    return attacks;
}

std::vector<bit::Bitboard> generate_diagonal_moves() {
    std::vector<bit::Bitboard> diagonal_moves(64);
    for (int sq = 0; sq < 64; ++sq) {
        diagonal_moves[sq] = calculate_all_diagonal_moves(sq);
    }
    return diagonal_moves;
}

std::vector<bit::Bitboard> diagonal_moves = generate_diagonal_moves();

bit::Bitboard compute_attack_for_occupancy(int sq, bit::Bitboard occupancy) {
    bit::Bitboard atk = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // North-East (Up-Right the board)
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; ++r, ++f) {
        bit::Bitboard b = 1ULL << (r * 8 + f);
        atk |= b;
        if (occupancy & b)
            break;
    }

    // North-West (Up-Left the board)
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; ++r, --f) {
        bit::Bitboard b = 1ULL << (r * 8 + f);
        atk |= b;
        if (occupancy & b)
            break;
    }

    // South-East (Down-Right across the board)
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; --r, ++f) {
        bit::Bitboard b = 1ULL << (r * 8 + f);
        atk |= b;
        if (occupancy & b)
            break;
    }

    // South-West (Down-Left across the board)
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; --r, --f) {
        bit::Bitboard b = 1ULL << (r * 8 + f);
        atk |= b;
        if (occupancy & b)
            break;
    }

    return atk;
}

std::vector<bit::Bitboard> get_occupancy_variations(int square) {
    bit::Bitboard atk_mask = diagonal_moves[square];
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
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6,
};

std::vector<bit::Bitboard> magic_numbers = {
	0xc085080200420200ULL,
	0x60014902028010ULL,
	0x401240100c201ULL,
	0x580ca104020080ULL,
	0x8434052000230010ULL,
	0x102080208820420ULL,
	0x2188410410403024ULL,
	0x40120805282800ULL,
	0x4420410888208083ULL,
	0x1049494040560ULL,
	0x6090100400842200ULL,
	0x1000090405002001ULL,
	0x48044030808c409ULL,
	0x20802080384ULL,
	0x2012008401084008ULL,
	0x9741088200826030ULL,
	0x822000400204c100ULL,
	0x14806004248220ULL,
	0x30200101020090ULL,
	0x148150082004004ULL,
	0x6020402112104ULL,
	0x4001000290080d22ULL,
	0x2029100900400ULL,
	0x804203145080880ULL,
	0x60a10048020440ULL,
	0xc08080b20028081ULL,
	0x1009001420c0410ULL,
	0x101004004040002ULL,
	0x1004405014000ULL,
	0x10029a0021005200ULL,
	0x4002308000480800ULL,
	0x301025015004800ULL,
	0x2402304004108200ULL,
	0x480110c802220800ULL,
	0x2004482801300741ULL,
	0x400400820a60200ULL,
	0x410040040040ULL,
	0x2828080020011000ULL,
	0x4008020050040110ULL,
	0x8202022026220089ULL,
	0x204092050200808ULL,
	0x404010802400812ULL,
	0x422002088009040ULL,
	0x180604202002020ULL,
	0x400109008200ULL,
	0x2420042000104ULL,
	0x40902089c008208ULL,
	0x4001021400420100ULL,
	0x484410082009ULL,
	0x2002051108125200ULL,
	0x22e4044108050ULL,
	0x800020880042ULL,
	0xb2020010021204a4ULL,
	0x2442100200802dULL,
	0x10100401c4040000ULL,
	0x2004a48200c828ULL,
	0x9090082014000ULL,
	0x800008088011040ULL,
	0x4000000a0900b808ULL,
	0x900420000420208ULL,
	0x4040104104ULL,
	0x120208c190820080ULL,
	0x4000102042040840ULL,
	0x8002421001010100ULL,
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
        int relevant_bits = __builtin_popcountll(diagonal_moves[square]); // Number of relevant bits for this square

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

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board) {
    bit::Bitboard relevant_occupancy = board.get_occupied_squares() & diagonal_moves[from];
    int index = magic_hash(relevant_occupancy, magic_numbers[from], shift_values[from]);
    return attack_table[from][index];
}

} // namespace diagonal
} // namespace moves
} // namespace chess_engine