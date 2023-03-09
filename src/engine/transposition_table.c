#include "transposition_table.h"

#include <stdlib.h>
#include <assert.h>

void ClearTTEntries(TranspositionTable_t* table) {
    for(int i = 0; i < table->numEntries; i++) {
        table->entries[i].key = 0;
        InitMove(&table->entries[i].bestMove);
    }
}

void TranspositionTableInit(TranspositionTable_t* table, Megabytes_t megabytes) {
    Bytes_t entrySize = sizeof(TTEntry_t);
    Bytes_t bytes = megabytes * bytes_per_megabyte;

    table->numEntries = bytes / entrySize;

    table->entries = malloc(table->numEntries * entrySize);
    assert(table->entries != NULL);

    ClearTTEntries(table);
}

TTIndex_t GetTTIndex(TranspositionTable_t* table, ZobristHash_t hash) {
    return hash % table->numEntries;
}

TTEntry_t GetTTEntry(TranspositionTable_t* table, TTIndex_t index) {
    return table->entries[index];
}

bool TTHit(TTEntry_t entry, ZobristHash_t hash) {
    return entry.key == (TTKey_t)hash;
}

TTFlag_t DetermineTTFlag(EvalScore_t bestScore, EvalScore_t oldAlpha, EvalScore_t alpha, EvalScore_t beta) {
    if(bestScore >= beta) {
        return lower_bound;
    } else if(alpha != oldAlpha) {
        return exact;
    } else {
        return upper_bound;
    }
}

void StoreTTEntry(
    TranspositionTable_t* table,
    TTIndex_t index,
    TTFlag_t flag,
    Depth_t depth,
    Move_t bestMove,
    EvalScore_t bestScore,
    ZobristHash_t hash
)
{
    table->entries[index].flag = flag;
    table->entries[index].depth = depth;
    table->entries[index].bestMove = bestMove;
    table->entries[index].bestScore = bestScore;
    table->entries[index].key = (TTKey_t)hash;
}

bool TTCutoffIsPossible(TTEntry_t entry, EvalScore_t alpha, EvalScore_t beta, Depth_t currentDepth) {
    if(entry.depth < currentDepth) {
        return false;
    }

    switch(entry.flag) {
        case exact:
            return true;
        case lower_bound:
            return entry.bestScore >= beta;
        case upper_bound:
            return entry.bestScore <= alpha;
        default:
            return false;
    }
}

void TeardownTT(TranspositionTable_t* table) {
    free(table->entries);
    table->entries = NULL;
}