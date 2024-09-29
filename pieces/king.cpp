#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "../utils.h"
#include <array>
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace king {

constexpr std::array<bit::Bitboard, 64> calculate_king_moves() {
    std::array<bit::Bitboard, 64> moves = {0ULL}; // Initialize all moves to zero
    const std::array<int, 8> directions = {+1, -1, -7, -8, -9, +7, +8, +9};

    for (int sq = 0; sq < 64; ++sq) {
        bit::Bitboard b = 0ULL;
        int file = sq % 8;

        for (int direction : directions) {
            int move = sq + direction;

            // Check if move stays within the board
            if (move < 0 || move >= 64)
                continue;

            // Handle left edge (file 0) and right edge (file 7) conditions
            int new_file = move % 8;
            if ((file == 0 && (direction == -1 || direction == -9 || direction == +7)) ||
                (file == 7 && (direction == +1 || direction == +9 || direction == -7))) {
                continue;
            }

            // Add valid move to the bitboard
            b |= 1ULL << move;
        }
        moves[sq] = b;
    }

    return moves;
}

// Precompute the king moves for all 64 squares
constexpr std::array<bit::Bitboard, 64> king_moves = calculate_king_moves();

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    bit::Bitboard curr_king = board.get_king(color);

    // Return 0 if there's no king at the given position
    if (((1ULL << from) & curr_king) == 0) {
        return 0ULL;
    }

    bit::Bitboard available_king_moves = king_moves[static_cast<int>(from)];
    bit::Bitboard valid_squares = (color == piece::Color::WHITE) ? ~board.get_white_pieces() : ~board.get_black_pieces();
    bit::Bitboard legal_moves = available_king_moves & valid_squares;

    // Filter out moves where the king would be in check
    bit::Bitboard safe_moves = 0ULL;
    for (int sq = 0; sq < 64; ++sq) {
        if (legal_moves & (1ULL << sq)) {
            // Temporarily make the move and check if the king would be in check
            board::Board temp_board = board;
            temp_board.move_piece(from, sq, piece::KING, color);
            if (!game_state.is_square_attacked(sq, utils::opposite_color(color))) {
                safe_moves |= (1ULL << sq); // Only allow safe moves
            }
        }
    }

    // Handle castling
    if (color == piece::Color::WHITE && from == 5) {
        if (game_state.white_castle_kingside &&
            !game_state.is_square_attacked(square::E1, piece::Color::BLACK) &&
            !game_state.is_square_attacked(square::F1, piece::Color::BLACK) &&
            !game_state.is_square_attacked(square::G1, piece::Color::BLACK) &&
            !board.is_occupied(square::F1) && !board.is_occupied(square::G1)) {
            safe_moves |= (1ULL << square::G1); // Kingside castling
        }
        if (game_state.white_castle_queenside &&
            !game_state.is_square_attacked(square::E1, piece::Color::BLACK) &&
            !game_state.is_square_attacked(square::D1, piece::Color::BLACK) &&
            !game_state.is_square_attacked(square::C1, piece::Color::BLACK) &&
            !board.is_occupied(square::D1) && !board.is_occupied(square::C1) &&
            !board.is_occupied(square::B1)) {
            safe_moves |= (1ULL << square::C1); // Queenside castling
        }
    } else if (color == piece::Color::BLACK && from == 60) {
        if (game_state.black_castle_kingside &&
            !game_state.is_square_attacked(square::E8, piece::Color::WHITE) &&
            !game_state.is_square_attacked(square::F8, piece::Color::WHITE) &&
            !game_state.is_square_attacked(square::G8, piece::Color::WHITE) &&
            !board.is_occupied(square::F8) &&
            !board.is_occupied(square::G8)) {
            safe_moves |= (1ULL << square::G8); // Kingside castling
        }
        if (game_state.black_castle_queenside &&
            !game_state.is_square_attacked(square::E8, piece::Color::WHITE) &&
            !game_state.is_square_attacked(square::D8, piece::Color::WHITE) &&
            !game_state.is_square_attacked(square::C8, piece::Color::WHITE) &&
            !board.is_occupied(square::D8) && !board.is_occupied(square::C8) &&
            !board.is_occupied(square::B8)) {
            safe_moves |= (1ULL << square::C8); // Queenside castling
        }
    }
    return safe_moves;
}

} // namespace king
} // namespace chess_engine
