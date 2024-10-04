#include "transposition.h"
#include "../enums.h"
#include <vector>

namespace chess_engine {
namespace transposition {

TranspositionTable::TranspositionTable(size_t size_mb) {
    size = (size_mb * 1024 * 1024) / sizeof(TTEntry);
    table.resize(size);
    clear();
}

TranspositionTable::~TranspositionTable() {}

void TranspositionTable::store(uint64_t key, int depth, int score, NodeType type, const moves::Move &best_move) {
    size_t index = key % size;
    TTEntry &entry = table[index];

    // Replace if new position is searched to a greater or equal depth
    if (depth >= entry.depth) {
        entry.key = key;
        entry.depth = depth;
        entry.score = score;
        entry.type = type;
        entry.best_move = best_move;
    }
}

bool TranspositionTable::probe(uint64_t key, int depth, int &score, NodeType &type, moves::Move &best_move) {
    size_t index = key % size;
    TTEntry &entry = table[index];

    if (entry.key == key && entry.depth >= depth) {
        score = entry.score;
        type = entry.type;
        best_move = entry.best_move;
        return true;
    }

    return false;
}

void TranspositionTable::clear() {
    for (auto &entry : table) {
        entry.key = 0;
        entry.depth = -1;
        entry.score = 0;
        entry.type = NodeType::EXACT;
        entry.best_move = moves::Move();
    }
}

} // namespace transposition
} // namespace chess_engine