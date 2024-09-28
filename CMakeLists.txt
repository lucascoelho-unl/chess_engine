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
    main.cpp
    utils.cpp
    structure/board.cpp
    structure/game_state.cpp
    moves/moves.cpp
    moves/slide/diagonal.cpp
    moves/slide/straight.cpp
    pieces/pawn.cpp
    pieces/knight.cpp
    pieces/bishop.cpp
    pieces/rook.cpp
    pieces/queen.cpp
    pieces/king.cpp
)
