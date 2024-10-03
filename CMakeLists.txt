cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories for headers
include_directories(
    ${PROJECT_SOURCE_DIR}/structure
    ${PROJECT_SOURCE_DIR}/moves
    ${PROJECT_SOURCE_DIR}/moves/slide
    ${PROJECT_SOURCE_DIR}/pieces
)

# List all the source files and add them to the executable target
add_executable(chess_engine
    chess_backend/main.cpp
    chess_backend/utils.cpp
    chess_backend/structure/board.cpp
    chess_backend/structure/game_state.cpp
    chess_backend/structure/square.cpp
    chess_backend/moves/moves.cpp
    chess_backend/moves/slide/diagonal.cpp
    chess_backend/moves/slide/straight.cpp
    chess_backend/pieces/pawn.cpp
    chess_backend/pieces/knight.cpp
    chess_backend/pieces/bishop.cpp
    chess_backend/pieces/rook.cpp
    chess_backend/pieces/queen.cpp
    chess_backend/pieces/king.cpp
    chess_backend/generator/evaluate.cpp
    chess_backend/generator/search.cpp
    chess_backend/generator/order.cpp
)