#include "enums.h"
#include "generator/evaluate.h"
#include "generator/search.h"
#include "generator/transposition.h"
#include "generator/zobrist.h"
#include "moves/moves.h"
#include "structure/game_state.h"
#include "structure/square.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

// namespace chess_engine {
// namespace search {

// transposition::TranspositionTable tt(64); // 64 MB table

// } // namespace search
// } // namespace chess_engine

// using namespace chess_engine;

// // Function to print the board and evaluations
// void print_board_and_eval(game_state::GameState &state, piece::Color color) {
//     std::cout << state.get_board().to_string() << std::endl;

//     int eval_func = evaluate::evaluate(color, state);

//     // Get evaluation from transposition table
//     uint64_t hash = zobrist::compute_hash(state);
//     int tt_score;
//     transposition::NodeType tt_type;
//     moves::Move tt_move;
//     bool is_valid_probing = search::tt.probe(hash, 0, tt_score, tt_type, tt_move);
//     int tt_eval = is_valid_probing ? tt_score : 0;

//     std::cout << "Evaluation function: " << eval_func << std::endl;
//     if (tt_type == transposition::NodeType::EXACT) {
//         std::cout << "Transposition table evaluation: " << tt_eval << std::endl;
//         std::cout << "Found position in transposition table!" << std::endl;
//         std::cout << "Evaluation: " << tt_score << std::endl;
//         std::cout << "Hash: " << hash << std::endl;
//         std::cout << moves::to_string(tt_move) << std::endl;
//         std::cout << std::endl;
//     }
//     // std::this_thread::sleep_for(std::chrono::seconds(5));
// }

// int main() {
//     // Initialize Zobrist keys
//     zobrist::init_zobrist_keys();

//     // Set initial FEN for the starting position
//     // std::string fen = "Bk6/B7/8/8/8/8/8/K b KQkq - 0 1";
//     // std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
//     std::string fen = "rn1q1rk1/pbppbppp/1p2pn2/8/2PP4/2N2NP1/PPQ1PPBP/R1B1K2R b KQ - 0 1";

//     // Initialize game state from FEN
//     game_state::GameState game_state = game_state::set_game_state(fen);

//     // Game loop
//     piece::Color current_turn = game_state.turn;
//     int move_number = 1;
//     int max_moves = 150; // Limit the number of moves to prevent infinite games

//     while (!game_state.is_game_over() && move_number <= max_moves) {

//         // Measure time taken to find the best move
//         auto start = std::chrono::high_resolution_clock::now();

//         // Find the best move for the current player
//         moves::Move best_move = search::find_best_move(4, current_turn, game_state);

//         auto end = std::chrono::high_resolution_clock::now();
//         auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//         std::cout << "Move " << move_number << " ("
//                   << (current_turn == piece::Color::WHITE ? "White" : "Black")
//                   << " to move):" << std::endl;

//         print_board_and_eval(game_state, current_turn);

//         // std::cout << "Time taken for move: " << duration.count() << " ms" << std::endl;

//         // // Make the best move
//         // std::cout << "Best move: " << square::int_position_to_string(best_move.from)
//         //           << " to " << square::int_position_to_string(best_move.to) << std::endl;

//         if (!game_state.make_move(best_move)) {
//             std::cout << moves::to_string(best_move) << std::endl;
//         }

//         // Switch turns
//         current_turn = utils::opposite_color(current_turn);
//         move_number++;

//         // Add a small delay to make the output readable
//         // std::this_thread::sleep_for(std::chrono::milliseconds(10000));
//     }

//     // Print final board state
//     std::cout << "Final board state:" << std::endl;
//     print_board_and_eval(game_state, current_turn);

//     if (game_state.is_game_over()) {
//         std::cout << "Game over!" << std::endl;
//         // You can add more detailed end-game information here
//     } else {
//         std::cout << "Maximum number of moves reached." << std::endl;
//     }

//     return 0;
// }

namespace beast = boost::beast;   // From <boost/beast.hpp>
namespace http = beast::http;     // From <boost/beast/http.hpp>
namespace net = boost::asio;      // From <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // From <boost/asio/ip/tcp.hpp>
using namespace chess_engine;

namespace chess_engine {
namespace search {

transposition::TranspositionTable tt(64); // 64 MB table

} // namespace search
} // namespace chess_engine

// Function to handle CORS and respond to POST requests
void handle_request(http::request<http::string_body> &&req, http::response<http::string_body> &res) {
    if (req.method() == http::verb::options) {
        // Handle preflight request (CORS)
        res.result(http::status::no_content);
        res.set(http::field::access_control_allow_origin, "*");
        res.set(http::field::access_control_allow_methods, "POST, OPTIONS");
        res.set(http::field::access_control_allow_headers, "Content-Type, Authorization");
        return;
    }

    if (req.method() == http::verb::post) {
        // Parse the JSON body
        std::string body = req.body();
        std::istringstream iss(body);
        boost::property_tree::ptree pt;

        // Parse the JSON body into a property tree
        try {
            boost::property_tree::read_json(iss, pt);

            // Extract the FEN string
            std::string fen = pt.get<std::string>("fen");

            // Calculate the best move from the FEN string
            moves::Move best_move = search::calculate_best_move(fen);

            // Convert the move positions to chess notation strings using square::int_position_to_string
            std::string from_str = square::int_position_to_string(best_move.from);
            std::string to_str = square::int_position_to_string(best_move.to);

            // Respond with the move in JSON format
            std::string response_body = "{\"from\": \"" + from_str + "\", \"to\": \"" + to_str + "\"}";
            res.body() = response_body;
            res.set(http::field::content_type, "application/json");
            res.set(http::field::access_control_allow_origin, "*"); // Handle CORS
            res.prepare_payload();
            res.result(http::status::ok);
        } catch (const std::exception &e) {
            // Handle JSON parsing errors
            res.result(http::status::bad_request);
            res.body() = "Invalid JSON format";
            res.prepare_payload();
        }
        return;
    }

    res.result(http::status::bad_request);
    res.body() = "Invalid request";
    res.prepare_payload();
}

// This function will run a single connection session
void do_session(tcp::socket socket) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;

        // Read the request
        http::read(socket, buffer, req);

        // Create a response
        http::response<http::string_body> res;

        // Handle the request and generate a response
        handle_request(std::move(req), res);

        // Send the response
        http::write(socket, res);
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int main() {
    // Initialize Zobrist keys
    zobrist::init_zobrist_keys();

    try {
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 18080;

        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};

        std::cout << "Server is running on port 18080.\n";

        for (;;) {
            // Wait for a connection
            acceptor.accept(socket);

            // Handle the session
            std::thread([socket = std::move(socket)]() mutable {
                do_session(std::move(socket));
            }).detach();
        }
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}
