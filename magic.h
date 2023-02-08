#ifndef __MAGIC_H__
#define __MAGIC_H__

#include <stdint.h>

#include "board_constants.h"

#define NUM_HASH_ENTRIES 107648

typedef Bitboard_t MagicBB_t;
typedef uint32_t Hash_t;

typedef struct {
    Bitboard_t mask;
    MagicBB_t magic;
    uint8_t shift;
    uint32_t offset;
} MagicEntry_t;

void InitAllMagicEntries(
    MagicEntry_t rookMagicEntries[NUM_SQUARES],
    MagicEntry_t bishopMagicEntries[NUM_SQUARES],
    Bitboard_t hashTable[NUM_HASH_ENTRIES]
);

#define MagicHash(blockers, magic, shift) (blockers * magic) >> shift

#define GetSlidingAttackSet(magicEntry, blockers) \
    magicEntry.hashTable[ MagicHash(blockers, magicEntry.magic, magicEntry.shift) ]

#endif