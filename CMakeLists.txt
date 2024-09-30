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
    engine/main.cpp
    engine/utils.cpp
    engine/structure/board.cpp
    engine/structure/game_state.cpp
    engine/structure/square.cpp
    engine/moves/moves.cpp
    engine/moves/slide/diagonal.cpp
    engine/moves/slide/straight.cpp
    engine/pieces/pawn.cpp
    engine/pieces/knight.cpp
    engine/pieces/bishop.cpp
    engine/pieces/rook.cpp
    engine/pieces/queen.cpp
    engine/pieces/king.cpp
    engine/generator/evaluate.cpp
    engine/generator/search.cpp
)
