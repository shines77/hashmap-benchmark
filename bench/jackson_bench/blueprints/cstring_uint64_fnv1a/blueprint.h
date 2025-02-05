// /bench/jackson_bench/blueprints/cstring_uint64_fnv1a/blueprint.h
// Copyright (c) 2024 Jackson L. Allan.
// Distributed under the MIT License (see the accompanying LICENSE file).

#pragma once

#include <memory.h>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstring>
#include <memory>

#include "bench_config.h"

#define CSTRING_UINT64_FNV1A_ENABLED

struct cstring_uint64_fnv1a
{
    using key_type = char *;
    using value_type = std::uint64_t;
    using element_type = std::pair<const key_type, value_type>;

    static constexpr const char * name = "cstring_uint64_fnv1a";
    static constexpr const char * label = "16-char c-string key, 64-bit value";

    static constexpr std::size_t string_length = 16;

    static constexpr std::size_t get_data_size()
    {
        return (BENCHMARK_TOTAL_BYTES / (sizeof(element_type) + sizeof(key_type) * KEY_ACTUAL));
    }

    // FNV-1a.
    static inline std::uint64_t hash_key(const key_type & key)
    {
        std::size_t hash = 0xcbf29ce484222325ull;
        char * c = key;
        while (*c) {
            hash = ((unsigned char)*c++ ^ hash) * 0x100000001b3ull;
        }

        return hash;
    }

    static inline bool cmpr_keys(const key_type & key_1, const key_type & key_2)
    {
        return (::strcmp(key_1, key_2) == 0);
    }

    // Fills the keys array with pointers to strings stored in one contiguous block of memory.
    // This approach makes initialization faster, but it does mean that operations involving the keys will benefit from
    // some artificial cache locality (compared to separately allocated strings).
    static void fill_unique_keys(std::vector<key_type> & keys)
    {
        static std::vector<char> backing_data;

        backing_data.resize(keys.size() * string_length);

        char current[string_length];
        std::memset(current, 'a', string_length - 1);
        current[string_length - 1] = '\0';

        for (std::size_t i = 0; i < keys.size(); ++i) {
            keys[i] = backing_data.data() + i * string_length;

            ::memcpy(keys[i], current, string_length);

            for (std::size_t j = 0; j < string_length - 1; ++j) {
                if (++current[j] <= 'z')
                    break;

                current[j] = 'a';
            }
        }
    }
};
