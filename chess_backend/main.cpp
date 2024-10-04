#include "enums.h"
#include "generator/search.h"
#include "generator/transposition.h"
#include "generator/zobrist.h"
#include "moves/moves.h"
#include "structure/square.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace beast = boost::beast;   // From <boost/beast.hpp>
namespace http = beast::http;     // From <boost/beast/http.hpp>
namespace net = boost::asio;      // From <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // From <boost/asio/ip/tcp.hpp>
using namespace chess_engine;

// Global transposition table
transposition::TranspositionTable tt(64); // 64 MB table

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
