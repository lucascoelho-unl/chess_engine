#include "zobrist.h"
#include <random>

namespace chess_engine {
namespace zobrist {

std::array<std::array<uint64_t, 64>, 12> piece_keys;
std::array<uint64_t, 16> castling_keys;
std::array<uint64_t, 8> en_passant_keys;
uint64_t side_to_move_key;

void init_zobrist_keys() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    for (auto &piece_type : piece_keys) {
        for (auto &square : piece_type) {
            square = dis(gen);
        }
    }

    for (auto &key : castling_keys) {
        key = dis(gen);
    }

    for (auto &key : en_passant_keys) {
        key = dis(gen);
    }

    side_to_move_key = dis(gen);
}

uint64_t compute_hash(const game_state::GameState &state) {
    uint64_t hash = 0;

    // Hash pieces
    const board::Board &board = state.get_board();
    for (int sq = 0; sq < 64; ++sq) {
        piece::Type type = board.get_piece_type(sq);
        piece::Color color = board.get_piece_color(sq);
        if (type != piece::Type::EMPTY) {
            int piece_index = 2 * type + color;
            hash ^= piece_keys[piece_index][sq];
        }
    }

    // Hash castling rights
    int castling_index = 0;
    if (state.white_castle_kingside)
        castling_index |= 1;
    if (state.white_castle_queenside)
        castling_index |= 2;
    if (state.black_castle_kingside)
        castling_index |= 4;
    if (state.black_castle_queenside)
        castling_index |= 8;
    hash ^= castling_keys[castling_index];

    // Hash en passant
    if (state.en_passant_square != -1) {
        int file = state.en_passant_square % 8;
        hash ^= en_passant_keys[file];
    }

    // Hash side to move
    if (state.turn == piece::Color::BLACK) {
        hash ^= side_to_move_key;
    }

    return hash;
}

} // namespace zobrist
} // namespace chess_engine