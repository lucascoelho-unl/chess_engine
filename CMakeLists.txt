cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chess_engine
    main.cpp
    utils.cpp
    structure/board.cpp
    moves/moves.cpp
    moves/slide/diagonal.cpp
    moves/slide/straight.cpp
    pieces/pawn.cpp
    pieces/knight.cpp
    pieces/bishop.cpp
    pieces/rook.cpp
    pieces/queen.cpp
    pieces/king.cpp
    # other source files...
)