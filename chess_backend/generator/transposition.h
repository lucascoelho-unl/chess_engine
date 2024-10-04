#ifndef CHESS_ENGINE_TRANSPOSITION_H
#define CHESS_ENGINE_TRANSPOSITION_H

#include "../enums.h"
#include "../moves/moves.h"
#include <cstdint>

namespace chess_engine {
namespace transposition {

struct TTEntry {
    uint64_t key;          // Zobrist hash of the position
    int depth;             // Depth of the search
    int score;             // Evaluation score
    NodeType type;         // Type of node (EXACT, ALPHA, or BETA)
    moves::Move best_move; // Best move found for this position
};

// Declare the transposition table class (implementation will be in .cpp file)
class TranspositionTable {
  public:
    TranspositionTable(size_t size_mb);
    ~TranspositionTable();

    void store(uint64_t key, int depth, int score, NodeType type, const moves::Move &best_move);
    bool probe(uint64_t key, int depth, int &score, NodeType &type, moves::Move &best_move);
    void clear();

  private:
    std::vector<TTEntry> table;
    size_t size;
};

} // namespace transposition
} // namespace chess_engine

#endif