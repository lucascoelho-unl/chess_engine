#ifndef CHESS_ENGINE_GAME_STATE_H
#define CHESS_ENGINE_GAME_STATE_H

#include "../enums.h"
#include "../moves/moves.h"
#include "../utils.h"
#include "bitboard.h"
#include "board.h"
#include <deque>
#include <iostream>
#include <string>

namespace chess_engine {
namespace game_state {

class GameState {
  public:
    board::Board board;          // The current state of the chessboard
    piece::Color turn;           // Whose turn it is (White or Black)
    bool white_castle_kingside;  // White's kingside castling rights
    bool white_castle_queenside; // White's queenside castling rights
    bool black_castle_kingside;  // Black's kingside castling rights
    bool black_castle_queenside; // Black's queenside castling rights
    int en_passant_square;       // Square where en passant is possible (-1 if not possible)
    int halfmove_clock;          // Number of half-moves since the last pawn move or capture
    int fullmove_number;         // The current move number (increases after Black's move)

    bool operator==(const GameState &other) const {
        return board == other.board &&
               turn == other.turn &&
               white_castle_kingside == other.white_castle_kingside &&
               white_castle_queenside == other.white_castle_queenside &&
               black_castle_kingside == other.black_castle_kingside &&
               black_castle_queenside == other.black_castle_queenside &&
               en_passant_square == other.en_passant_square &&
               halfmove_clock == other.halfmove_clock &&
               fullmove_number == other.fullmove_number;
    }

    // Stack to store previous game states (useful for unmaking moves)
    std::deque<moves::Reversible_Move> move_history;

    GameState() = default; // Default constructor

    GameState(const board::Board &board, piece::Color turn, bool w_k_castle, bool w_q_castle,
              bool b_k_castle, bool b_q_castle, int en_passant, int halfmove, int fullmove);

    bool is_checkmate();
    bool is_stalemate();
    bool is_draw_by_fifty_move_rule();
    bool is_game_over();
    bool is_square_attacked(int sq, piece::Color color) const;
    void switch_turn();
    void update_castling_rights(int from, int to);
    void update_en_passant(int from, int to);
    bool is_castling_valid(int from, int to, piece::Color color) const;
    const bool is_in_check(piece::Color color) const;
    GameState copy() const;

    bool make_move(moves::Move move);
    bool make_pseudo_move(moves::Move move);
    bool unmake_move();

    // Const version of get_board (read-only access)
    const board::Board &get_board() const {
        return board;
    }

    // Non-const version of get_board (for modification)
    board::Board &get_board() {
        return board;
    }
};

// Function declarations
GameState set_game_state(const std::string &fen);

} // namespace game_state
} // namespace chess_engine

#endif