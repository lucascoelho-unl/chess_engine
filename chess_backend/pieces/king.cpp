#include "king.h"
#include "../enums.h"
#include "../moves/moves.h"
#include "../structure/bitboard.h"
#include "../structure/game_state.h"
#include "../structure/square.h"
#include "../utils.h"
#include "pawn.h"
#include <array>
#include <sstream>
#include <stdexcept>

namespace chess_engine {
namespace king {

bit::Bitboard get_moves(int from, piece::Color color, const board::Board &board, const game_state::GameState &game_state) {
    bit::Bitboard curr_king = board.get_king(color);

    // Return 0 if there's no king at the given position
    if (((1ULL << from) & curr_king) == 0) {
        return 0ULL;
    }

    bit::Bitboard available_king_moves = king_moves[static_cast<int>(from)];
    bit::Bitboard valid_squares = (color == piece::Color::WHITE) ? ~board.get_white_pieces() : ~board.get_black_pieces();
    bit::Bitboard legal_moves = available_king_moves & valid_squares;

    int opponents_king_position = __builtin_ffsll(board.get_king(utils::opposite_color(color))) - 1;
    bit::Bitboard opponents_king_attacks = king_moves[opponents_king_position];

    bit::Bitboard opponents_pawn_attacks = pawn::get_possible_attacks(utils::opposite_color(color), game_state.get_board().get_pawns(utils::opposite_color(color)));

    bit::Bitboard opponent_attack_mask_without_king = moves::get_all_piece_moves(utils::opposite_color(color), board, game_state, true);
    bit::Bitboard opponent_attack_mask = opponent_attack_mask_without_king | opponents_king_attacks | opponents_pawn_attacks;

    bit::Bitboard safe_moves = legal_moves & ~opponent_attack_mask;

    // Handle castling
    if (color == piece::Color::WHITE && from == 4) {
        if (game_state.is_castling_valid(from, 6, piece::Color::WHITE)) {
            safe_moves |= (1ULL << square::G1); // Kingside castling
        }
        if (game_state.is_castling_valid(from, 2, piece::Color::WHITE)) {
            safe_moves |= (1ULL << square::C1); // Queenside castling
        }
    } else if (color == piece::Color::BLACK && from == 60) {
        if (game_state.is_castling_valid(from, 62, piece::Color::BLACK)) {
            safe_moves |= (1ULL << square::G8); // Kingside castling
        }
        if (game_state.is_castling_valid(from, 58, piece::Color::BLACK)) {
            safe_moves |= (1ULL << square::C8); // Queenside castling
        }
    }

    return safe_moves;
}

} // namespace king
} // namespace chess_engine
