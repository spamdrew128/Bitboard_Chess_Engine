#ifndef __ENGINE_TYPES_H__
#define __ENGINE_TYPES_H__

#include <stdint.h>
#include <stddef.h>

typedef int32_t EvalScore_t;
typedef int32_t Centipawns_t;

typedef uint8_t Depth_t;
typedef uint8_t Ply_t;
typedef uint64_t NodeCount_t;

typedef size_t Bytes_t;
typedef size_t Megabytes_t;
enum {
    bytes_per_megabyte = 1000
};

#endif
