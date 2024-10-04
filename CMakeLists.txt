cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include VCPKG toolchain
set(CMAKE_TOOLCHAIN_FILE /Users/lucascoelho/Desktop/chess_engine/build/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

# Set CMAKE_PREFIX_PATH to the general VCPKG share directory
set(CMAKE_PREFIX_PATH "/Users/lucascoelho/Desktop/chess_engine/build/vcpkg/installed/arm64-osx/share")

# Manually set ASIO include directory
set(ASIO_INCLUDE_DIR "/Users/lucascoelho/Desktop/chess_engine/build/vcpkg/installed/arm64-osx/include")

# Ensure that CMake knows where the asio headers are
include_directories(${ASIO_INCLUDE_DIR})

# Find Crow (Crow relies on asio, so we need to find Crow)
find_package(Crow CONFIG REQUIRED)

# Set the policy to avoid FindBoost warnings
cmake_policy(SET CMP0167 NEW)

# Find Boost (ensure you specify required components)
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Include directories for Boost
include_directories(${Boost_INCLUDE_DIRS})

# Add include directories for headers
include_directories(
    ${PROJECT_SOURCE_DIR}/structure
    ${PROJECT_SOURCE_DIR}/moves
    ${PROJECT_SOURCE_DIR}/moves/slide
    ${PROJECT_SOURCE_DIR}/pieces
    ${PROJECT_SOURCE_DIR}/generator
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
    chess_backend/generator/transposition.cpp
    chess_backend/generator/zobrist.cpp
)

# Link Crow, Boost, and pthread to the chess_engine executable (all using the keyword signature)
target_link_libraries(chess_engine PRIVATE Crow::Crow Boost::system Boost::filesystem pthread)
