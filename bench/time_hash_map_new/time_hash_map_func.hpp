
#ifndef TIME_HASH_MAP_FUNC_HPP
#define TIME_HASH_MAP_FUNC_HPP

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#include <cstdint>
#include <cstddef>

template <class MapType, class Vector>
static void map_serial_find(char const * title, std::size_t iters,
                            const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;
    mapped_type i;
    mapped_type max_iters = static_cast<mapped_type>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(indices[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    char info[128]; info[0] = '\0';
    ::snprintf(info, sizeof(info), "counter_r = %" PRIuSIZE, r);
    report_result(title, ut, lf, iters, 0, 0, info);
}

template <class MapType>
static void map_serial_find_success(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    std::vector<mapped_type> v(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        v[i] = i + 1;
    }

    map_serial_find<MapType>("serial_find_success", iters, v);
}

template <class MapType>
static void map_serial_find_random(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    std::vector<mapped_type> v(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        v[i] = i + 1;
    }

    shuffle_vector(v);

    map_serial_find<MapType>("serial_find_random", iters, v);
}

template <class MapType>
static void map_serial_find_failed(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;
    mapped_type i;
    mapped_type max_iters = static_cast<mapped_type>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (i = max_iters; i < max_iters * 2; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(i) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    char info[128]; info[0] = '\0';
    ::snprintf(info, sizeof(info), "counter_r = %" PRIuSIZE, r);
    report_result("serial_find_failed", ut, lf, iters, 0, 0, info);
}

template <class MapType>
static void map_serial_find_empty(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;
    mapped_type i;
    mapped_type max_iters = static_cast<mapped_type>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    hashmap.clear();

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(i) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    char info[128]; info[0] = '\0';
    ::snprintf(info, sizeof(info), "counter_r = %" PRIuSIZE, r);
    report_result("serial_find_empty", ut, lf, iters, 0, 0, info);
}

template <class MapType>
static void map_serial_insert(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.insert(std::make_pair(i, i + 1));
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_insert", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_insert_predicted(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        hashmap.reserve(max_iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.insert(std::make_pair(i, i + 1));
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_insert_predicted", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_insert_replace(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.insert(std::make_pair(i, i + 1));
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.insert(std::make_pair(i, i + 2));
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_insert_replace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_emplace(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(i, i + 1);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_emplace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_emplace_predicted(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        hashmap.reserve(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(i, i + 1);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_emplace_predicted", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_emplace_replace(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(i, i + 2);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_emplace_replace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_operator(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap[i] = i + 1;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial operator []", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_operator_predicted(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        hashmap.reserve(max_iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap[i] = i + 1;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial operator [] predicted", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_operator_replace(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap[i] = i + 1;
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap[i] = i + 2;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial operator [] replace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_erase(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.erase(i);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_erase", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_erase_failed(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = max_iters; i < max_iters * 2; i++) {
            hashmap.erase(i);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_erase_failed", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_toggle(std::size_t iters) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(i, i + 1);
            hashmap.erase(i);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("serial_toggle", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void map_serial_iterate(std::size_t iters) {
    typedef typename MapType::mapped_type       mapped_type;
    typedef typename MapType::const_iterator    const_iterator;

    MapType hashmap;
    jtest::StopWatch sw;
    mapped_type r;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        r = 1;
        reset_counter();
        sw.start();
        const_iterator it = hashmap.cbegin(), it_end = hashmap.cend();
        for (; it != it_end; ++it) {
            r ^= it->second;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // keep compiler from optimizing away r (we never call rand())
        ::srand(static_cast<unsigned int>(r));

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "r = %" PRIuSIZE ", hashmap.size() = %" PRIuSIZE "\n",
                   std::size_t(r), (std::size_t)hashmap.size());
        report_result("serial_emplace - iterate", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_find(char const * title, std::size_t iters,
                            const Vector & indices, const Vector & findIndices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;
    mapped_type i;
    mapped_type max_iters = static_cast<mapped_type>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap.emplace(indices[i], i);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(findIndices[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    char info[128]; info[0] = '\0';
    ::snprintf(info, sizeof(info), "counter_r = %" PRIuSIZE, r);
    report_result(title, ut, lf, iters, 0, 0, info);
}

template <class MapType, class Vector>
static void map_random_find_serial(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    std::vector<mapped_type> v(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        v[i] = i + 1;
    }

    map_random_find<MapType>("random_find_serial", iters, indices, v);
}

template <class MapType, class Vector>
static void map_random_find_random(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    std::vector<mapped_type> v(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        v[i] = i + 1;
    }

    shuffle_vector(v);

    map_random_find<MapType>("random_find_random", iters, indices, v);
}

template <class MapType, class Vector>
static void map_random_find_failed(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;
    mapped_type i;
    mapped_type max_iters = static_cast<mapped_type>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap.emplace(indices[i], i + 1);
    }

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(indices[i] + max_iters) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    char info[128]; info[0] = '\0';
    ::snprintf(info, sizeof(info), "counter_r = %" PRIuSIZE, r);
    report_result("random_find_failed", ut, lf, iters, 0, 0, info);
}

template <class MapType, class Vector>
static void map_random_find_empty(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;
    std::size_t r;
    mapped_type i;
    mapped_type max_iters = static_cast<mapped_type>(iters);

    for (i = 0; i < max_iters; i++) {
        hashmap.emplace(i, i + 1);
    }

    hashmap.clear();

    r = 1;
    reset_counter();
    sw.start();
    for (i = 0; i < max_iters; i++) {
        r ^= static_cast<std::size_t>(hashmap.find(indices[i]) != hashmap.end());
    }
    sw.stop();
    double ut = sw.getElapsedSecond();

    // keep compiler from optimizing away r (we never call rand())
    ::srand(static_cast<unsigned int>(r));

    double lf = hashmap.load_factor();
    char info[128]; info[0] = '\0';
    ::snprintf(info, sizeof(info), "counter_r = %" PRIuSIZE, r);
    report_result("random_find_empty", ut, lf, iters, 0, 0, info);
}

template <class MapType, class Vector>
static void map_random_insert(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.insert(std::make_pair(indices[i], i));
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_insert", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_insert_predicted(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        hashmap.reserve(max_iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.insert(std::make_pair(indices[i], i));
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_insert_predicted", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_insert_replace(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.insert(std::make_pair(indices[i], i));
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.insert(std::make_pair(indices[i], i + 1));
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_insert_replace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_emplace(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(indices[i], i);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_emplace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_emplace_predicted(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        hashmap.reserve(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(indices[i], i);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_emplace_predicted", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_emplace_replace(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(indices[i], i);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(indices[i], i + 2);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_emplace_replace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_operator(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap[indices[i]] = i;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random operator []", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_operator_predicted(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        hashmap.reserve(max_iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap[indices[i]] = i;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random operator [] predicted", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_operator_replace(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;

    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap[indices[i]] = i;
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap[indices[i]] = i + 1;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random operator [] replace", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_erase(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(indices[i], i);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.erase(indices[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_erase", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_erase_failed(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    MapType hashmap;
    jtest::StopWatch sw;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(indices[i], i);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.erase(indices[i] + max_iters);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_erase_failed", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_toggle(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type mapped_type;

    std::size_t start = CurrentMemoryUsage();
    MapType hashmap;

    {
        jtest::StopWatch sw;

        mapped_type max_iters = static_cast<mapped_type>(iters);

        reset_counter();
        sw.start();
        for (mapped_type i = 0; i < max_iters; i++) {
            hashmap.emplace(indices[i], i);
            hashmap.erase(indices[i]);
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "hashmap.size() = %" PRIuSIZE, (std::size_t)hashmap.size());
        report_result("random_toggle", ut, lf, iters, start, finish, info);
    }
}

template <class MapType, class Vector>
static void map_random_iterate(std::size_t iters, const Vector & indices) {
    typedef typename MapType::mapped_type       mapped_type;
    typedef typename MapType::const_iterator    const_iterator;

    MapType hashmap;

    jtest::StopWatch sw;
    mapped_type r;

    mapped_type max_iters = static_cast<mapped_type>(iters);
    for (mapped_type i = 0; i < max_iters; i++) {
        hashmap.emplace(indices[i], i);
    }

    std::size_t start = CurrentMemoryUsage();
    {
        r = 1;
        reset_counter();
        sw.start();
        const_iterator it = hashmap.cbegin(), it_end = hashmap.cend();
        for (; it != it_end; ++it) {
            r ^= it->second;
        }
        sw.stop();

        double ut = sw.getElapsedSecond();
        std::size_t finish = CurrentMemoryUsage();

        __COMPILER_BARRIER();

        // keep compiler from optimizing away r (we never call rand())
        ::srand(static_cast<unsigned int>(r));

        // Ensure the HashMap is not destructed
        ::srand(static_cast<unsigned int>(hashmap.size()));

        double lf = hashmap.load_factor();
        char info[128]; info[0] = '\0';
        ::snprintf(info, sizeof(info), "r = %" PRIuSIZE ", hashmap.size() = %" PRIuSIZE "\n",
                   std::size_t(r), (std::size_t)hashmap.size());
        report_result("random_emplace - iterate", ut, lf, iters, start, finish, info);
    }
}

template <class MapType>
static void stress_hash_function(std::size_t desired_insertions,
                                 std::size_t map_size,
                                 std::size_t stride) {
    jtest::StopWatch sw;
    std::uint32_t num_insertions = 0;
    // One measurement of user time (in seconds) is done for each iteration of
    // the outer loop.  The times are summed.
    double total_seconds = 0;
    const std::size_t k = desired_insertions / map_size;

    MapType hashmap;
    for (std::size_t o = 0; o < k; o++) {
        hashmap.clear();
        hashmap.rehash(map_size);
        sw.start();
        const std::size_t maxint = (1ull << (sizeof(uint32_t) * 8 - 1)) - 1;
        // Use n arithmetic sequences.  Using just one may lead to overflow
        // if stride * map_size > maxint.  Compute n by requiring
        // stride * map_size/n < maxint, i.e., map_size/(maxint/stride) < n
        char * key;   // something we can do math on
        const std::size_t n = map_size / (maxint / stride) + 1;
        for (std::size_t i = 0; i < n; i++) {
            key = nullptr;
            key += i;
            for (std::size_t j = 0; j < map_size / n; j++) {
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

#endif // TIME_HASH_MAP_FUNC_HPP
