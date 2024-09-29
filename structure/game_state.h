#ifndef CHESS_ENGINE_GAME_STATE_H
#define CHESS_ENGINE_GAME_STATE_H

#include "../enums.h"
#include "../moves/moves.h"
#include "../utils.h"
#include "bitboard.h"
#include "board.h"
#include <iostream>
#include <stack>
#include <string>

namespace chess_engine {
namespace game_state {

class Game_State {
  public:
    board::Board current_board;  // The current state of the chessboard
    piece::Color turn;           // Whose turn it is (White or Black)
    bool white_castle_kingside;  // White's kingside castling rights
    bool white_castle_queenside; // White's queenside castling rights
    bool black_castle_kingside;  // Black's kingside castling rights
    bool black_castle_queenside; // Black's queenside castling rights
    int en_passant_square;       // Square where en passant is possible (-1 if not possible)
    int halfmove_clock;          // Number of half-moves since the last pawn move or capture
    int fullmove_number;         // The current move number (increases after Black's move)

    bool operator==(const Game_State &other) const {
        return current_board == other.current_board &&
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
    std::stack<Game_State> previous_states;

    Game_State() = default; // Default constructor

    Game_State(const board::Board &board, piece::Color turn, bool w_k_castle, bool w_q_castle,
               bool b_k_castle, bool b_q_castle, int en_passant, int halfmove, int fullmove);

    bool is_square_attacked(int sq, piece::Color color) const;
    void save_state();
    void restore_previous_state();
    void switch_turn();
    void update_castling_rights(int from, int to);
    void update_en_passant(int from, int to);
    bool is_castling_valid(int from, int to, piece::Color color);

    bool make_move(int from, int to, piece::Type piece_type, piece::Color color, moves::Type move_type, piece::Type promotion);
    bool unmake_move();

    const board::Board &get_board() const { return current_board; }
};

// Function declarations
Game_State set_game_state(const std::string &fen);

} // namespace game_state
} // namespace chess_engine

#endif