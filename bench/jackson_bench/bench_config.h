//
// Copyright (c) 2024 Jackson L. Allan.
// Distributed under the MIT License (see the accompanying LICENSE file).
//

#ifndef JSTD_BENCH_JACKSON_BENCH_CONFIG_H
#define JSTD_BENCH_JACKSON_BENCH_CONFIG_H

#pragma once

#include <jstd/basic/stddef.h>

//
// Total benchmark data size (bytes) of key and value, for any key type and any value type.
//
#ifndef _DEBUG
#define BENCHMARK_TOTAL_BYTES  (64 * 1024 * 1024)
#else
#define BENCHMARK_TOTAL_BYTES  (1024 * 1024)
#endif

// The randomize test keys actual usage length.
#define KEY_ACTUAL  1

// The randomize test keys scale times.
#define KEY_SCALE   3

// The total number of unique keys to include in the benchmarks.
#define KEY_COUNT   200000

// The frequency at which to measure and record times.
// This must be a factor of KEY_COUNT.
#define KEY_COUNT_MEASUREMENT_INTERVAL  2000

// The number of erase key.
#define NUMS_ERASE_KEY      (KEY_COUNT_MEASUREMENT_INTERVAL / 2)

// The number of insert key when test erase().
#define NUMS_INSERT_KEY     KEY_COUNT_MEASUREMENT_INTERVAL

// The number of times to repeat the benchmarks.
#ifndef _DEBUG
#define RUN_COUNT   5
#else
#define RUN_COUNT   1
#endif

// Each data point in the outputted graphs is the average of the measurements for that point across all runs,
// excluding the lowest and highest measurements.
// This macro specifies the total number of measurements to discard (e.g. 2 means that the lowest measurement
// and highest measurement will be discarded).
// It should be an even number below RUN_COUNT.
// This mechanism reduces the impact of background noise interfering with the benchmarks.
#define DISCARDED_RUNS_COUNT    2

// The maximum load factor that all shims should, in theory, apply when initializing their maps.
// In reality, some hash map libraries do not allow custom load factors.
#define MAX_LOAD_FACTOR         0.875

// The size (in bytes) of the buffer to iterate over between maps in order to reset the state of the L1, L2,
// and L3 CPU caches.
// This mechanism is intended to ensure that each map starts the benchmarks under approximately the same
// condition with regard to whether the keys are cached.
#ifndef _DEBUG
#define APPROXIMATE_CACHE_SIZE  (32 * 1024 * 1024)
#else
#define APPROXIMATE_CACHE_SIZE  (4 * 1024 * 1024)
#endif

// The amount of time to sleep between benchmarks, Unit: ms.
#ifndef _DEBUG
#define MILLISECOND_COOLDOWN_BETWEEN_BENCHMARKS     1000
#else
#define MILLISECOND_COOLDOWN_BETWEEN_BENCHMARKS     100
#endif

// The specific benchmarks to run (comment them out to disable them).
#define BENCHMARK_FIND_EXISTING
#define BENCHMARK_FIND_NON_EXISTING
#define BENCHMARK_INSERT_EXISTING
#define BENCHMARK_INSERT_NON_EXISTING
//#define BENCHMARK_REPLACE_EXISTING
#define BENCHMARK_ERASE_EXISTING
#define BENCHMARK_ERASE_NON_EXISTING
#define BENCHMARK_ITERATION

// Blueprint slots.
#define BLUEPRINT_1         uint32_uint32_murmur
#define BLUEPRINT_2         uint64_uint64_murmur
#define BLUEPRINT_3         uint64_struct448_murmur
#define BLUEPRINT_4         cstring_uint64_fnv1a
// #define BLUEPRINT_5
// #define BLUEPRINT_6
// #define BLUEPRINT_7
// #define BLUEPRINT_8
// #define BLUEPRINT_9
// #define BLUEPRINT_10
// #define BLUEPRINT_11
// #define BLUEPRINT_12
// #define BLUEPRINT_13
// #define BLUEPRINT_14
// #define BLUEPRINT_15
// #define BLUEPRINT_16

// Hashmap slots.
#ifndef _DEBUG
//#define HASHMAP_1       std_unordered_map
#endif
#if !defined(_MSC_VER)
//#define HASHMAP_2       ska_bytell_hash_map
#endif
#define HASHMAP_3       ska_flat_hash_map
#if !defined(_MSC_VER)
#define HASHMAP_4       tsl_robin_map
#endif
#define HASHMAP_5       jstd_robin_hash_map
#if !defined(_MSC_VER) || (_MSC_VER >= 2000)
#define HASHMAP_6       ankerl_unordered_dense
#endif
#if (jstd_cplusplus >= 2017L)
#define HASHMAP_7       absl_flat_hash_map
#endif
#define HASHMAP_8       emilib2o
#define HASHMAP_9       jstd_group16_flat_map
#define HASHMAP_10      jstd_group15_flat_map
#if (jstd_cplusplus >= 2020L)
#define HASHMAP_11      boost_unordered_flat_map
#endif
// #define HASHMAP_12
// #define HASHMAP_13
// #define HASHMAP_14
// #define HASHMAP_15
// #define HASHMAP_16

#endif // JSTD_BENCH_JACKSON_BENCH_CONFIG_H
