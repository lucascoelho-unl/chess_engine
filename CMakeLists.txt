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
    src/main.cpp
    src/utils.cpp
    src/structure/board.cpp
    src/structure/game_state.cpp
    src/structure/square.cpp
    src/moves/moves.cpp
    src/moves/slide/diagonal.cpp
    src/moves/slide/straight.cpp
    src/pieces/pawn.cpp
    src/pieces/knight.cpp
    src/pieces/bishop.cpp
    src/pieces/rook.cpp
    src/pieces/queen.cpp
    src/pieces/king.cpp
    src/generator/evaluate.cpp
    src/generator/search.cpp
)