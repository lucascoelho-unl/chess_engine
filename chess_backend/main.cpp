#include "enums.h"
#include "generator/evaluate.h"
#include "generator/search.h"
#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/game_state.h"
#include "structure/square.h"
#include "utils.h"
#include <chrono> // For timing
#include <iostream>
#include <string>
#include <thread>

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

const int INF = std::numeric_limits<int>::max();
const int NEG_INF = std::numeric_limits<int>::min();

// int main() {
//     // Initialize game state, etc.
//     std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/QNBQKBNQ w - - 0 1";
//     //     // std::string fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1NBQK2R w Kkq - 0 1";
//     //     // std::string fen = "rr2k3/8/8/8/8/8/8/7K w - - 0 1";
//     //     // std::string fen = "8/2q1P1k1/8/8/8/8/8/K7 w - - 0 1";
//     //     // std::string fen = "rnqbbnkr/pppppppp/8/8/8/8/PPPPPPPP/RNQBBNKR w - - 0 1";

//     //     // Create the game state from the FEN string
//     game_state::Game_State game_state = game_state::set_game_state(fen);

//     int evaluation = 0;
//     if (game_state.turn == piece::Color::WHITE) {
//         evaluation = chess_engine::search::minimax(3, NEG_INF, INF, true, game_state);
//     } else {
//         evaluation = chess_engine::search::minimax(3, NEG_INF, INF, false, game_state);
//     }
//     std::cout << "Evaluation: " << evaluation << std::endl;

//     return 0;
// }

int main() {
    // Initial FEN for starting position
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
    // std::string fen = "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1NBQK2R w Kkq - 0 1";
    // std::string fen = "rr2k3/8/8/8/8/8/8/7K w - - 0 1";
    // std::string fen = "8/2q1P1k1/8/8/8/8/8/K7 w - - 0 1";
    // std::string fen = "rnqbbnkr/pppppppp/8/8/8/8/PPPPPPPP/RNQBBNKR w - - 0 1";

    // Create the game state from the FEN string
    game_state::Game_State game_state = game_state::set_game_state(fen);

    // Play a game loop where the engine plays for both White and Black
    while (true) {
        print_board(game_state);
        // Display board evaluation after the move
        int evaluation = evaluate::evaluate(game_state);
        std::cout << "Current board evaluation: " << evaluation << std::endl;

        // Determine whose turn it is
        piece::Color current_color = game_state.turn;
        std::string player = (current_color == piece::Color::WHITE) ? "White" : "Black";
        std::cout << player << "'s turn (Engine)...\n";

        // Start timing
        auto start_time = std::chrono::high_resolution_clock::now();

        moves::Move best_move = search::find_best_move(3, current_color, game_state);

        // End timing
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;

        // Output the time taken
        std::cout << "Time taken for engine to find the best move: " << elapsed_time.count() << " seconds.\n";

        // Output the engine's move
        std::string from_square_str = square::int_position_to_string(best_move.from);
        std::string to_square_str = square::int_position_to_string(best_move.to);
        std::string piece_type_str = utils::piece_type_to_string(best_move.piece_type);
        std::string move_type_str = utils::move_type_to_string(best_move.move_type);

        std::cout << player << " move: " << piece_type_str << " from " << from_square_str << " to " << to_square_str << " (" << move_type_str << ")\n";

        // Execute the move
        if (!game_state.make_move(best_move)) {
            std::cout << player << " made an invalid move! Something went wrong.\n";
            break;
        }

        // Check for game over
        if (game_state.is_game_over()) {
            if (game_state.is_draw_by_fifty_move_rule()) {
                std::cout << "Game over! Draw" << std::endl;
            } else {
                std::cout << "Game over! " << player << " wins!\n";
            }
            print_board(game_state);
            break;
        }

        // Pause for 2 seconds before the next move
        // std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
