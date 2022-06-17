
#ifndef TIME_HASH_MAP_HPP
#define TIME_HASH_MAP_HPP

#pragma once

namespace v1 {

template <class MapType, class Vector>
static void time_map_find(char const * title, std::size_t iters,
                          const Vector & indices) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;
    std::uint32_t r;
    std::uint32_t i;
    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::uint32_t>(hashmap.find(indices[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    ::srand(static_cast<unsigned int>(r));   // keep compiler from optimizing away r (we never call rand())
    report_result(title, ut, iters, 0, 0);
}

template <class MapType>
static void time_map_find_sequential(std::size_t iters) {
    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    std::vector<std::uint32_t> v(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        v[i] = i + 1;
    }

    time_map_find<MapType>("map_find_sequential", iters, v);
}

template <class MapType>
static void time_map_find_random(std::size_t iters) {
    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    std::vector<std::uint32_t> v(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        v[i] = i + 1;
    }

    shuffle_vector(v);

    time_map_find<MapType>("map_find_random", iters, v);
}

template <class MapType>
static void time_map_find_failed(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;
    std::uint32_t r;
    std::uint32_t i;
    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    r = 1;
    reset_counter();
    sw.start();
    for (i = max_iters; i < max_iters * 2; i++) {
        r ^= static_cast<std::uint32_t>(hashmap.find(i) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    ::srand(static_cast<unsigned int>(r));   // keep compiler from optimizing away r (we never call rand())
    report_result("map_find_failed", ut, iters, 0, 0);
}

template <class MapType>
static void time_map_find_empty(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;
    std::uint32_t r;
    std::uint32_t i;
    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::uint32_t>(hashmap.find(i) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    ::srand(static_cast<unsigned int>(r));   // keep compiler from optimizing away r (we never call rand())
    report_result("map_find_empty", ut, iters, 0, 0);
}

template <class MapType>
static void time_map_insert(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap.insert(std::make_pair(i, i + 1));
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_insert", ut, iters, start, finish);
}

template <class MapType>
static void time_map_insert_predicted(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    const std::size_t start = CurrentMemoryUsage();

    hashmap.resize(iters);

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap.insert(std::make_pair(i, i + 1));
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_insert_predicted", ut, iters, start, finish);
}

template <class MapType>
static void time_map_insert_replace(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap.insert(std::make_pair(i, i + 1));
    }

    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap.insert(std::make_pair(i, i + 2));
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_insert_replace", ut, iters, start, finish);
}

template <class MapType>
static void time_map_emplace(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_emplace", ut, iters, start, finish);
}

template <class MapType>
static void time_map_emplace_predicted(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    const std::size_t start = CurrentMemoryUsage();

    hashmap.resize(iters);

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_emplace_predicted", ut, iters, start, finish);
}

template <class MapType>
static void time_map_emplace_replace(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 2;
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_emplace_replace", ut, iters, start, finish);
}

template <class MapType>
static void time_map_erase(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap.erase(i);
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_erase", ut, iters, start, finish);
}

template <class MapType>
static void time_map_erase_failed(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = max_iters; i < max_iters * 2; i++) {
        hashmap.erase(i);
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_erase_failed", ut, iters, start, finish);
}

template <class MapType>
static void time_map_toggle(std::size_t iters) {
    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    const std::size_t start = CurrentMemoryUsage();

    reset_counter();
    sw.start();
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
        hashmap.erase(i);
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    report_result("map_toggle", ut, iters, start, finish);
}

template <class MapType>
static void time_map_iterate(std::size_t iters) {
    typedef typename MapType::const_iterator const_iterator;

    MapType hashmap(kInitCapacity);
    jtest::StopWatch sw;
    std::uint32_t r;

    std::uint32_t max_iters = static_cast<std::uint32_t>(iters);
    for (std::uint32_t i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    const std::size_t start = CurrentMemoryUsage();

    r = 1;
    reset_counter();
    sw.start();
    for (const_iterator it = hashmap.begin(); it != hashmap.end(); ++it) {
        r ^= static_cast<uint32_t>(it->second);
    }
    sw.stop();

    double ut = sw.getElapsedSecond();
    const std::size_t finish = CurrentMemoryUsage();
    ::srand(static_cast<unsigned int>(r));   // keep compiler from optimizing away r (we never call rand())
    report_result("map_iterate", ut, iters, start, finish);
}

template <class MapType>
static void stress_hash_function(std::size_t desired_insertions,
                                 std::size_t map_size,
                                 std::size_t stride) {
    jtest::StopWatch sw;
    uint32_t num_insertions = 0;
    // One measurement of user time (in seconds) is done for each iteration of
    // the outer loop.  The times are summed.
    double total_seconds = 0;
    const uint32_t k = (uint32_t)(desired_insertions / map_size);

    MapType hashmap(kInitCapacity);
    for (uint32_t o = 0; o < k; o++) {
        hashmap.clear();
        hashmap.resize(map_size);
        sw.start();
        const uint32_t maxint = (1ull << (sizeof(uint32_t) * 8 - 1)) - 1;
        // Use n arithmetic sequences.  Using just one may lead to overflow
        // if stride * map_size > maxint.  Compute n by requiring
        // stride * map_size/n < maxint, i.e., map_size/(maxint/stride) < n
        char * key;   // something we can do math on
        const uint32_t n = (uint32_t)(map_size / ((std::size_t)maxint / stride) + 1);
        for (uint32_t i = 0; i < n; i++) {
            key = nullptr;
            key += i;
            for (uint32_t j = 0; j < map_size / n; j++) {
                key += stride;
                hashmap[reinterpret_cast<typename MapType::key_type>(key)]
                        = ++num_insertions;
            }
        }
        sw.stop();
        total_seconds += sw.getElapsedSecond();
    }

    if (num_insertions != 0) {
        printf("stress_hash_function: map_size = %-4" PRIuPTR " stride = %-4" PRIuPTR ": %.2f ns/insertion\n",
               map_size, stride, total_seconds * 1e9 / num_insertions);
    }
}

template <class MapType>
static void stress_hash_function(std::size_t num_inserts) {
    static const std::size_t kMapSizes [] = { 256, 1024 };
    std::size_t len = sizeof(kMapSizes) / sizeof(kMapSizes[0]);
    for (std::size_t i = 0; i < len; i++) {
        const std::size_t map_size = kMapSizes[i];
        for (std::size_t stride = 1; stride <= map_size; stride *= map_size) {
            stress_hash_function<MapType>(num_inserts, map_size, stride);
        }
    }
}

template <class MapType, class StressMapType>
static void measure_hashmap(const char * name, std::size_t obj_size, std::size_t entry_size,
                            std::size_t iters, bool is_stress_hash_function) {
    if (entry_size == 0) {
        printf("%s (%" PRIuPTR " byte objects, %" PRIuPTR " byte ValueType, %" PRIuPTR " iterations):\n",
               name, obj_size, sizeof(typename MapType::value_type), iters);
    }
    else {
        printf("%s (%" PRIuPTR " byte objects, %" PRIuPTR " byte EntryType, %" PRIuPTR " iterations):\n",
               name, obj_size, entry_size, iters);
    }
    if (1) printf("\n");

    if (1) time_map_find_sequential<MapType>(iters);
    if (1) time_map_find_random<MapType>(iters);
    if (1) time_map_find_failed<MapType>(iters);
    if (1) time_map_find_empty<MapType>(iters);
    if (1) printf("\n");

    if (1) time_map_insert<MapType>(iters);
    if (1) time_map_insert_predicted<MapType>(iters);
    if (1) time_map_insert_replace<MapType>(iters);
    if (1) printf("\n");

    if (1) time_map_emplace<MapType>(iters);
    if (1) time_map_emplace_predicted<MapType>(iters);
    if (1) time_map_emplace_replace<MapType>(iters);
    if (1) printf("\n");

    if (1) time_map_erase<MapType>(iters);
    if (1) time_map_erase_failed<MapType>(iters);
    if (1) time_map_toggle<MapType>(iters);
    if (1) time_map_iterate<MapType>(iters);

    if (1) printf("\n");

    // This last test is useful only if the map type uses hashing.
    // And it's slow, so use fewer iterations.
    if (is_stress_hash_function) {
        // Blank line in the output makes clear that what follows isn't part of the
        // table of results that we just printed.
        stress_hash_function<StressMapType>(iters / 4);
        printf("\n");
    }
}

template <typename HashObj, typename Value>
static void test_all_hashmaps(std::size_t obj_size, std::size_t iters) {
    const bool is_stress_hash_function = (obj_size <= 8);

    if (FLAGS_test_std_hash_map) {
        measure_hashmap<StdHashMap<HashObj,   Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>>,
                        StdHashMap<HashObj *, Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "stdext::hash_map<K, V>", obj_size, 0, iters, is_stress_hash_function);
    }

    if (FLAGS_test_std_unordered_map) {
        measure_hashmap<StdUnorderedMap<HashObj,   Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>>,
                        StdUnorderedMap<HashObj *, Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "std::unordered_map<K, V>", obj_size, 0, iters, is_stress_hash_function);
    }

    if (FLAGS_test_jstd_flat16_hash_map) {
        typedef jstd::flat16_hash_map<HashObj, Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>> flat16_hash_map;
        measure_hashmap<jstd::flat16_hash_map<HashObj,   Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>>,
                        jstd::flat16_hash_map<HashObj *, Value, HashFn<typename HashObj::key_type, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "jstd::flat16_hash_map<K, V>", obj_size,
            sizeof(typename flat16_hash_map::node_type), iters, is_stress_hash_function);
    }
}

void benchmark_all_hashmaps(std::size_t iters)
{
    // It would be nice to set these at run-time, but by setting them at
    // compile-time, we allow optimizations that make it as fast to use
    // a HashObject as it would be to use just a straight int/char
    // buffer.  To keep memory use similar, we normalize the number of
    // iterations based on size.
    if (FLAGS_test_4_bytes) {
        test_all_hashmaps<HashObject<std::uint32_t, 4, 4>, std::uint32_t>(4, iters / 1);
    }

    if (FLAGS_test_8_bytes) {
        test_all_hashmaps<HashObject<std::size_t, 8, 8>, std::size_t>(8, iters / 2);
    }

    if (FLAGS_test_16_bytes) {
        test_all_hashmaps<HashObject<std::size_t, 16, 16>, std::size_t>(16, iters / 4);
    }

    if (FLAGS_test_256_bytes) {
        test_all_hashmaps<HashObject<std::size_t, 256, 32>, std::size_t>(256, iters / 32);
    }
}

} // namespace v1

#endif // TIME_HASH_MAP_HPP
