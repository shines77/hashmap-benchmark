// /bench/jackson_bench/blueprints/uint64_uint64_murmur/blueprint.h
// Copyright (c) 2024 Jackson L. Allan.
// Distributed under the MIT License (see the accompanying LICENSE file).

#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <numeric>

#include "bench_config.h"

#define UINT64_UINT64_MURMUR_ENABLED

struct uint64_uint64_murmur
{
    using key_type = std::uint64_t;
    using value_type = std::uint64_t;
    using element_type = std::pair<const key_type, value_type>;

    static constexpr const char * name = "uint64_uint64_murmur";
    static constexpr const char * label = "64-bit integer key, 64-bit value";

    static constexpr std::size_t get_data_size()
    {
        return (BENCHMARK_TOTAL_BYTES / (sizeof(element_type) + sizeof(key_type) * KEY_ACTUAL));
    }

    // MurmurHash3’s 64-bit finalizer.
    static inline std::uint64_t hash_key(const key_type & key)
    {
        std::uint64_t result = key;
        result ^= result >> 33;
        result *= 0xff51afd7ed558ccdull;
        result ^= result >> 33;
        result *= 0xc4ceb9fe1a85ec53ull;
        result ^= result >> 33;
        return result;
    }

    static inline bool cmpr_keys(const key_type & key_1, const key_type & key_2)
    {
        return (key_1 == key_2);
    }

    static void fill_unique_keys(std::vector<key_type> & keys)
    {
        std::iota(keys.begin(), keys.end(), 0);
    }
};
