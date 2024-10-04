#include "../enums.h"
#include "../structure/board.h"
#include "../structure/game_state.h"

namespace chess_engine {
namespace evaluate {

constexpr int WHITE_PAWN = 2 * piece::Type::PAWN + piece::Color::WHITE;
constexpr int BLACK_PAWN = 2 * piece::Type::PAWN + piece::Color::BLACK;
constexpr int WHITE_KNIGHT = 2 * piece::Type::KNIGHT + piece::Color::WHITE;
constexpr int BLACK_KNIGHT = 2 * piece::Type::KNIGHT + piece::Color::BLACK;
constexpr int WHITE_BISHOP = 2 * piece::Type::BISHOP + piece::Color::WHITE;
constexpr int BLACK_BISHOP = 2 * piece::Type::BISHOP + piece::Color::BLACK;
constexpr int WHITE_ROOK = 2 * piece::Type::ROOK + piece::Color::WHITE;
constexpr int BLACK_ROOK = 2 * piece::Type::ROOK + piece::Color::BLACK;
constexpr int WHITE_QUEEN = 2 * piece::Type::QUEEN + piece::Color::WHITE;
constexpr int BLACK_QUEEN = 2 * piece::Type::QUEEN + piece::Color::BLACK;
constexpr int WHITE_KING = 2 * piece::Type::KING + piece::Color::WHITE;
constexpr int BLACK_KING = 2 * piece::Type::KING + piece::Color::BLACK;
constexpr int EMPTY = BLACK_KING + 1;

constexpr int PCOLOR(int p) {
    return p & 1;
}

constexpr int FLIP(int sq) {
    return sq ^ 56;
}

constexpr int OTHER(int side) {
    return side ^ 1;
}

constexpr int get_piece_value(piece::Type type, piece::Color color) {
    switch (type) {
    case piece::Type::PAWN:
        return (color == piece::Color::WHITE) ? WHITE_PAWN : BLACK_PAWN;
    case piece::Type::KNIGHT:
        return (color == piece::Color::WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
    case piece::Type::BISHOP:
        return (color == piece::Color::WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
    case piece::Type::ROOK:
        return (color == piece::Color::WHITE) ? WHITE_ROOK : BLACK_ROOK;
    case piece::Type::QUEEN:
        return (color == piece::Color::WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
    case piece::Type::KING:
        return (color == piece::Color::WHITE) ? WHITE_KING : BLACK_KING;
    default:
        return EMPTY; // If no valid piece type is passed, return EMPTY
    }
}

constexpr std::array<int, 6> mg_value = {82, 337, 365, 477, 1025, 0};
constexpr std::array<int, 6> eg_value = {94, 281, 297, 512, 936, 0};

// clang-format off
constexpr std::array<int, 64> mg_pawn_table = {
    0, 0, 0, 0, 0, 0, 0, 0,
    98, 134, 61, 95, 68, 126, 34, -11,
    -6, 7, 26, 31, 65, 56, 25, -20,
    -14, 13, 6, 21, 23, 12, 17, -23,
    -27, -2, -5, 12, 17, 6, 10, -25,
    -26, -4, -4, -10, 3, 3, 33, -12,
    -35, -1, -20, -23, -15, 24, 38, -22,
    0, 0, 0, 0, 0, 0, 0, 0
};

constexpr std::array<int, 64> eg_pawn_table = {
    0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
    94, 100,  85,  67,  56,  53,  82,  84,
    32,  24,  13,   5,  -2,   4,  17,  17,
    13,   9,  -3,  -7,  -7,  -8,   3,  -1,
    4,   7,  -6,   1,   0,  -5,  -1,  -8,
    13,   8,   8,  10,  13,   0,   2,  -7,
    0,   0,   0,   0,   0,   0,   0,   0,
};

constexpr std::array<int, 64> mg_knight_table = {
    -167, -89, -34, -49,  61, -97, -15, -107,
    -73, -41,  72,  36,  23,  62,   7,  -17,
    -47,  60,  37,  65,  84, 129,  73,   44,
    -9,  17,  19,  53,  37,  69,  18,   22,
    -13,   4,  16,  13,  28,  19,  21,   -8,
    -23,  -9,  12,  10,  19,  17,  25,  -16,
    -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};

constexpr std::array<int, 64> eg_knight_table = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};

constexpr std::array<int, 64> mg_bishop_table = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
    -4,   5,  19,  50,  37,  37,   7,  -2,
    -6,  13,  13,  26,  34,  12,  10,   4,
    0,  15,  15,  15,  14,  27,  18,  10,
    4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};

constexpr std::array<int, 64> eg_bishop_table = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
    -8,  -4,   7, -12, -3, -13,  -4, -14,
    2,  -8,   0,  -1, -2,   6,   0,   4,
    -3,   9,  12,   9, 14,  10,   3,   2,
    -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

constexpr std::array<int, 64> mg_rook_table = {
    32,  42,  32,  51, 63,  9,  31,  43,
    27,  32,  58,  62, 80, 67,  26,  44,
    -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26,
};

constexpr std::array<int, 64> eg_rook_table = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
    7,  7,  7,  5,  4,  -3,  -5,  -3,
    4,  3, 13,  1,  2,   1,  -1,   2,
    3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};

constexpr std::array<int, 64> mg_queen_table = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
    -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
    -1, -18,  -9,  10, -15, -25, -31, -50,
};

constexpr std::array<int, 64> eg_queen_table = {
    -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
    3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};

constexpr std::array<int, 64> mg_king_table = {
    -65,  23,  16, -15, -56, -34,   2,  13,
    29,  -1, -20,  -7,  -8,  -4, -38, -29,
    -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
    1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};

constexpr std::array<int, 64> eg_king_table = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
    10,  17,  23,  15,  20,  45,  44,  13,
    -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};

constexpr std::array<std::array<int, 64>, 6> mg_pesto_table = {
    mg_pawn_table,
    mg_knight_table,
    mg_bishop_table,
    mg_rook_table,
    mg_queen_table,
    mg_king_table
};

constexpr std::array<std::array<int, 64>, 6> eg_pesto_table = {
    eg_pawn_table,
    eg_knight_table,
    eg_bishop_table,
    eg_rook_table,
    eg_queen_table,
    eg_king_table
};
// clang-format on

constexpr std::array<int, 12> gamephase_inc = {0, 0, 1, 1, 1, 1, 2, 2, 4, 4, 0, 0};

constexpr auto init_mg_table() {
    std::array<std::array<int, 64>, 12> mg_table{};
    for (int p = piece::Type::PAWN, pc = WHITE_PAWN; p <= piece::Type::KING; pc += 2, p++) {
        for (int sq = 0; sq < 64; sq++) {
            mg_table[pc][sq] = mg_value[p] + mg_pesto_table[p][sq];
            mg_table[pc + 1][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
        }
    }
    return mg_table;
}

constexpr auto init_eg_table() {
    std::array<std::array<int, 64>, 12> eg_table{};
    for (int p = piece::Type::PAWN, pc = WHITE_PAWN; p <= piece::Type::KING; pc += 2, p++) {
        for (int sq = 0; sq < 64; sq++) {
            eg_table[pc][sq] = eg_value[p] + eg_pesto_table[p][sq];
            eg_table[pc + 1][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
        }
    }
    return eg_table;
}

// Use the constexpr functions to initialize the tables
constexpr auto mg_table = init_mg_table();
constexpr auto eg_table = init_eg_table();

int piece_value(piece::Type type) {
    switch (type) {
    case piece::Type::PAWN:
        return 100;
    case piece::Type::KNIGHT:
        return 320;
    case piece::Type::BISHOP:
        return 330;
    case piece::Type::ROOK:
        return 500;
    case piece::Type::QUEEN:
        return 900;
    case piece::Type::KING:
        return 20000;
    }
    return 0;
}

int positional_score(piece::Color color, game_state::GameState &state) {
    board::Board board = state.get_board();
    int side_2_move = (color == piece::Color::WHITE) ? 0 : 1;

    std::array<int, 2> mg = {0, 0};
    std::array<int, 2> eg = {0, 0};
    int game_phase = 0;

    for (int sq = 0; sq < 64; ++sq) {
        piece::Type pt = board.get_piece_type(sq);
        piece::Color pc = board.get_piece_color(sq);
        int p = get_piece_value(pt, pc);

        if (p != EMPTY) {
            mg[PCOLOR(p)] += mg_table[p][sq];
            eg[PCOLOR(p)] += eg_table[p][sq];
            game_phase += gamephase_inc[p];
        }
    }
    int mg_score = mg[side_2_move] - mg[OTHER(side_2_move)];
    int eg_score = eg[side_2_move] - eg[OTHER(side_2_move)];
    int mg_phase = game_phase > 24 ? 24 : game_phase;
    int eg_phase = 24 - mg_phase;
    return (mg_score * mg_phase + eg_score * eg_phase) / 24;
}

int material_score(piece::Color color, game_state::GameState &state) {
    board::Board board = state.get_board();
    int white_material = 0;
    int black_material = 0;

    for (int sq = 0; sq < 64; ++sq) {
        piece::Type type = board.get_piece_type(sq);
        piece::Color p_color = board.get_piece_color(sq);

        if (type != piece::Type::EMPTY) {
            int material_value = piece_value(type); // Ensure material value is correct here
            if (p_color == piece::Color::WHITE) {
                white_material += material_value;
            } else {
                black_material += material_value;
            }
        }
    }
    return (color == piece::Color::WHITE) ? (white_material - black_material) : (black_material - white_material);
}

int pawn_structure_score(piece::Color color, game_state::GameState &state) {
    board::Board board = state.get_board();

    return 0;
}

int king_safety_score(piece::Color color, game_state::GameState &state) {
    board::Board board = state.get_board();

    return 0;
}

int evaluate(piece::Color color, game_state::GameState &state) {
    if (state.is_checkmate()) {
        return 9999999;
    }
    if (state.is_stalemate() || state.is_draw_by_fifty_move_rule()) {
        return 0;
    }

    // Evaluate from the perspective of the side to move
    int score = positional_score(color, state);
    score += material_score(color, state);
    score += pawn_structure_score(color, state);
    score += king_safety_score(color, state);

    return score;
}

} // namespace evaluate
} // namespace chess_engine