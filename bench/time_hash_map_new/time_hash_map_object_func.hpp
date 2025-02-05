
#ifndef TIME_HASH_MAP_OBJECT_FUNC_HPP
#define TIME_HASH_MAP_OBJECT_FUNC_HPP

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#include <cstdint>
#include <cstddef>

template <class MapType, class PairVector, class KeyVector>
static void map_serial_find_success(std::size_t iters, const PairVector & kvs, const KeyVector & keys) {
    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i].first, kvs[i].second);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (std::size_t i = 0; i < iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(keys[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    report_result("serial_find_success", ut, lf, iters, 0, 0);
    printf("counter_r = %" PRIuPTR "\n", r);
}

template <class MapType, class PairVector, class KeyVector>
static void map_random_find_success(std::size_t iters, const PairVector & kvs, const KeyVector & rnd_keys) {
    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i].first, kvs[i].second);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (std::size_t i = 0; i < iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(rnd_keys[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    report_result("random_find_success", ut, lf, iters, 0, 0);
    printf("counter_r = %" PRIuPTR "\n", r);
}

template <class MapType, class PairVector, class KeyVector>
static void map_find_failed(std::size_t iters, const PairVector & kvs, const KeyVector & miss_keys) {
    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i].first, kvs[i].second);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (std::size_t i = 0; i < iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(miss_keys[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    report_result("find_failed", ut, lf, iters, 0, 0);
    printf("counter_r = %" PRIuPTR "\n", r);
}

template <class MapType, class PairVector, class KeyVector>
static void map_find_empty(std::size_t iters, const PairVector & kvs, const KeyVector & keys) {
    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i].first, kvs[i].second);
    }

    hashmap.clear();

    r = 1;
    reset_counter();
    sw.start();
    for (std::size_t i = 0; i < iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(keys[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    report_result("find_empty", ut, lf, iters, 0, 0);
    printf("counter_r = %" PRIuPTR "\n", r);
}

template <class MapType, class PairVector>
static void map_insert(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.insert(kvs[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("insert", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_insert_predicted(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        hashmap.rehash(iters);

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.insert(kvs[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("insert_predicted", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_insert_replace(std::size_t iters, const PairVector & kvs) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.insert(kvs[i]);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.insert(kvs[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("insert_replace", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_emplace(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(kvs[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("emplace", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_emplace_predicted(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        hashmap.rehash(iters);

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(kvs[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("emplace_predicted", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_emplace_replace(std::size_t iters, const PairVector & kvs) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i]);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(kvs[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("emplace_replace", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_try_emplace(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(kvs[i].first, kvs[i].second);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("try_emplace", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_try_emplace_predicted(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        hashmap.rehash(iters);

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(kvs[i].first, kvs[i].second);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("try_emplace_predicted", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector, class KeyVector>
static void map_try_emplace_replace(std::size_t iters, const PairVector & kvs, const KeyVector & rnd_keys) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i].first, kvs[i].second);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(rnd_keys[i], kvs[i].second);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("try_emplace_replace", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_operator(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap[kvs[i].first] = kvs[i].second;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("operator []", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_operator_predicted(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        hashmap.rehash(iters);

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap[kvs[i].first] = kvs[i].second;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("operator [] predicted", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_operator_replace(std::size_t iters, const PairVector & kvs) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap[kvs[i].first] = kvs[i].second;
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap[kvs[i].first] = kvs[i].second;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("operator [] replace", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_serial_erase(std::size_t iters, const PairVector & kvs) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i]);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.erase(kvs[i].first);
        }
        assert(hashmap.size() == 0);
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("serial_erase", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector, class KeyVector>
static void map_random_erase(std::size_t iters, const PairVector & kvs, const KeyVector & rnd_keys) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i]);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.erase(rnd_keys[i]);
        }
        assert(hashmap.size() == 0);
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("random_erase", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector, class KeyVector>
static void map_erase_failed(std::size_t iters, const PairVector & kvs, const KeyVector & miss_keys) {
    MapType hashmap;
    jtest::StopWatch sw;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i]);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.erase(miss_keys[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("erase_failed", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_toggle(std::size_t iters, const PairVector & kvs) {
    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        reset_counter();
        sw.start();
        for (std::size_t i = 0; i < iters; i++) {
            hashmap.emplace(kvs[i]);
            hashmap.erase(kvs[i].first);
            assert(hashmap.size() == 0);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("toggle", ut, lf, iters, start, finish);
        printf("hashmap.size() = %" PRIuPTR "\n", hashmap.size());
    }
}

template <class MapType, class PairVector>
static void map_iterate(std::size_t iters, const PairVector & kvs) {
    typedef typename MapType::const_iterator    const_iterator;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;

    for (std::size_t i = 0; i < iters; i++) {
        hashmap.emplace(kvs[i]);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        r = 0;
        reset_counter();
        sw.start();
        const_iterator it = hashmap.cbegin(), it_end = hashmap.cend();
        for (; it != it_end; ++it) {
            r += it->second.size();
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // keep compiler from optimizing away r (we never call rand())
        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(r + hashmap.size()));

        double lf = hashmap.load_factor();
        report_result("iterate", ut, lf, iters, start, finish);
        printf("r = %" PRIuPTR ", hashmap.size() = %" PRIuPTR "\n", r, hashmap.size());
    }
}

#endif // TIME_HASH_MAP_OBJECT_FUNC_HPP
