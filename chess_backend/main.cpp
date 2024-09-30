#include "enums.h"
#include "generator/search.h"
#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/game_state.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>
#include <string>

using namespace chess_engine;

void print_board(const game_state::Game_State &game_state) {
    std::cout << game_state.get_board().to_string() << std::endl;
}

// Function to get the move type from the player's input
moves::Type get_move_type() {
    std::string move_type_input;
    std::cout << "Enter the move type (normal, capture, promotion, castling, en_passant): ";
    std::cin >> move_type_input;

    if (move_type_input == "normal") {
        return moves::Type::NORMAL;
    } else if (move_type_input == "capture") {
        return moves::Type::CAPTURE;
    } else if (move_type_input == "promotion") {
        return moves::Type::PROMOTION;
    } else if (move_type_input == "castling") {
        return moves::Type::CASTLING;
    } else if (move_type_input == "en_passant") {
        return moves::Type::EN_PASSANT;
    } else {
        std::cout << "Invalid move type. Defaulting to normal move.\n";
        return moves::Type::NORMAL;
    }
}

// Function to get the promotion type if the move is a promotion
piece::Type get_promotion_type() {
    std::string promotion_input;
    std::cout << "Enter the promotion piece (queen, rook, bishop, knight): ";
    std::cin >> promotion_input;

    if (promotion_input == "queen") {
        return piece::Type::QUEEN;
    } else if (promotion_input == "rook") {
        return piece::Type::ROOK;
    } else if (promotion_input == "bishop") {
        return piece::Type::BISHOP;
    } else if (promotion_input == "knight") {
        return piece::Type::KNIGHT;
    } else {
        std::cout << "Invalid promotion type. Defaulting to queen.\n";
        return piece::Type::QUEEN;
    }
}

int main() {
    // Initial FEN for starting position
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // std::string fen = "r1bq1rk1/pppp1pbp/2n2np1/4p3/2B1P3/2NPBN2/PPP2PPP/R2QK2R w KQ - 0 1";

    // Create the game state from the FEN string
    game_state::Game_State game_state = game_state::set_game_state(fen);

    // Play a game loop
    while (true) {
        print_board(game_state);

        // Player's turn (input move)
        std::string move_input;
        std::cout << "Your move (e.g., e2e4): ";
        std::cin >> move_input;

        // Convert input move to integers
        if (move_input.length() != 4) {
            std::cout << "Invalid input. Please enter a move in the format 'e2e4'.\n";
            continue;
        }

        int from_square = square::string_position_to_int(move_input.substr(0, 2));
        int to_square = square::string_position_to_int(move_input.substr(2, 2));

        // Get the piece type and color
        piece::Type piece_type = game_state.get_board().get_piece_type(from_square, game_state.turn);
        piece::Color player_color = game_state.turn;

        // Get the move type (normal, capture, promotion, etc.)
        moves::Type move_type = get_move_type();

        // Get promotion type if it's a promotion move
        piece::Type promotion = piece::Type::EMPTY;
        if (move_type == moves::Type::PROMOTION) {
            promotion = get_promotion_type();
        }

        // Create the player's move
        moves::Move player_move(from_square, to_square, piece_type, player_color, move_type, promotion);

        // Attempt to make the player's move
        if (!game_state.make_move(player_move)) {
            std::cout << "Invalid move. Try again.\n";
            continue;
        }

        // Check for game over
        if (game_state.is_game_over()) {
            std::cout << "Game over! You win!\n";
            break;
        }

        // AI's turn (engine finds the best move)
        std::cout << "Engine's turn...\n";
        moves::Move best_move = search::find_best_move(5, piece::Color::BLACK, game_state); // Assuming depth = 3 for search

        std::string from_square_str = square::int_position_to_string(best_move.from);
        std::string to_square_str = square::int_position_to_string(best_move.to);
        std::string piece_type_str = utils::piece_type_to_string(best_move.piece_type);
        std::string move_type_str = utils::move_type_to_string(best_move.move_type);

        std::cout << "Engine moves: " << piece_type_str << " from " << from_square_str << " to " << to_square_str << " (" << move_type_str << ")\n";

        // Execute the engine's move
        if (!game_state.make_move(best_move)) {
            std::cout << "Engine made an invalid move! Something went wrong.\n";
            break;
        }

        // Check if the engine won
        if (game_state.is_game_over()) {
            std::cout << "Game over! The engine wins!\n";
            break;
        }
    }

    return 0;
}
