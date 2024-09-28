#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include <sstream>
#include <stdexcept>
#include <string>

namespace chess_engine {
namespace pawn {

bit::Bitboard get_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    bit::Bitboard curr_position = 1ULL << from;
    bit::Bitboard valid_positions = board.get_empty_squares();
    bit::Bitboard moves = 0ULL;

    // Check if pawn exists in this position
    if (!(curr_position & board.get_pawns(color))) {
        return 0ULL;
    }

    // Regular forward moves (1 square or 2 squares)
    if (color == board::piece::WHITE) {
        bit::Bitboard p1 = (curr_position << 8) & valid_positions; // Move 1 square forward
        if (p1 != 0) {
            moves |= p1;
            if ((from >= 8 && from < 16) && ((curr_position << 16) & valid_positions)) { // Move 2 squares forward if on rank 2
                moves |= (curr_position << 16);
            }
        }
    } else {
        bit::Bitboard p1 = (curr_position >> 8) & valid_positions; // Move 1 square forward
        if (p1 != 0) {
            moves |= p1;
            if ((from >= 48 && from < 56) && ((curr_position >> 16) & valid_positions)) { // Move 2 squares forward if on rank 7
                moves |= (curr_position >> 16);
            }
        }
    }

    // Captures
    bit::Bitboard enemy_pieces = (color == board::piece::WHITE) ? board.get_black_pieces() : board.get_white_pieces();
    if (color == board::piece::WHITE) {
        // Capture diagonally left and right
        moves |= ((curr_position & ~board::a_file) << 7) & enemy_pieces; // Capture to the left
        moves |= ((curr_position & ~board::h_file) << 9) & enemy_pieces; // Capture to the right
    } else {
        // Capture diagonally left and right
        moves |= ((curr_position & ~board::h_file) >> 7) & enemy_pieces; // Capture to the left
        moves |= ((curr_position & ~board::a_file) >> 9) & enemy_pieces; // Capture to the right
    }

    // En passant capture
    if (game_state.en_passant_square != -1) {
        if (color == board::piece::WHITE) {
            if (from / 8 == 4) { // White pawns must be on rank 5 for en passant
                bit::Bitboard left_capture = (curr_position & ~board::a_file) << 7;
                bit::Bitboard right_capture = (curr_position & ~board::h_file) << 9;
                if (left_capture & (1ULL << game_state.en_passant_square)) {
                    moves |= left_capture;
                }
                if (right_capture & (1ULL << game_state.en_passant_square)) {
                    moves |= right_capture;
                }
            }
        } else {
            if (from / 8 == 3) { // Black pawns must be on rank 4 for en passant
                bit::Bitboard left_capture = (curr_position & ~board::h_file) >> 7;
                bit::Bitboard right_capture = (curr_position & ~board::a_file) >> 9;
                if (left_capture & (1ULL << game_state.en_passant_square)) {
                    moves |= left_capture;
                }
                if (right_capture & (1ULL << game_state.en_passant_square)) {
                    moves |= right_capture;
                }
            }
        }
    }

    // Pawn promotion: handled during move execution (no need to flag it here, just move to final rank)
    return moves;
}

} // namespace pawn
} // namespace chess_engine
