//
// Copyright (c) 2024 Jackson L. Allan.
// Distributed under the MIT License (see the accompanying LICENSE file).
//

#include "jstd/basic/stddef.h"

#include <stdlib.h>
#include <stdio.h>

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <thread>
#include <type_traits>
#include <algorithm>
#if JSTD_IS_CXX_20
#include <concepts>
#endif
#include "bench_config.h"

#include "jstd/test/StopWatch.h"
#include "jstd/system/Console.h"

//
// Variable printed before the program closes to prevent compiler from optimizing out function calls during the
// benchmarks.
// This approach proved to be more reliable than local volatile variables.
//
std::size_t do_not_optimize = 0;

// Standard stringification macro, used to form the file paths of the blueprints and shims.
#define STRINGIFY_(x)   #x
#define STRINGIFY(x)    STRINGIFY_(x)

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64))
    #define JSTD_UNALIGNED  __unaligned
#else
    #define JSTD_UNALIGNED
#endif

#ifdef __cplusplus
extern "C++"
{
    template <typename CountOfType, std::size_t SizeOfArray>
    char (*jstd_countof_helper(JSTD_UNALIGNED CountOfType (&_Array)[SizeOfArray]))[SizeOfArray];

    #define jstd_countof(_Array)   (sizeof(*jstd_countof_helper(_Array)) + 0)
}
#else
    #define jstd_countof(_Array)   (sizeof(_Array) / sizeof(_Array[0]))
#endif

// Concept to check that a blueprint is correctly formed.
#if JSTD_IS_CXX_20
template <typename BluePrint>
concept check_blueprint =
    std::is_object<typename BluePrint::value_type>::value &&
    std::same_as<decltype(BluePrint::label), const char * const> &&
    std::same_as<decltype(BluePrint::hash_key), uint64_t(const typename BluePrint::key_type &)> &&
    std::same_as<
        decltype(BluePrint::cmpr_keys),
        bool(const typename BluePrint::key_type &, const typename BluePrint::key_type &)
    > &&
    std::same_as<decltype(BluePrint::fill_unique_keys), void(std::vector<typename BluePrint::key_type> &)>
;
#define STATIC_ASSERT(expr) static_assert(expr)
#else
#define STATIC_ASSERT(expr)
#endif // JSTD_IS_CXX_20

// #include blueprints and check them for correctness.
#ifdef BLUEPRINT_1
#include STRINGIFY(blueprints/BLUEPRINT_1/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_1>);
#endif
#ifdef BLUEPRINT_2
#include STRINGIFY(blueprints/BLUEPRINT_2/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_2>);
#endif
#ifdef BLUEPRINT_3
#include STRINGIFY(blueprints/BLUEPRINT_3/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_3>);
#endif
#ifdef BLUEPRINT_4
#include STRINGIFY(blueprints/BLUEPRINT_4/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_4>);
#endif
#ifdef BLUEPRINT_5
#include STRINGIFY(blueprints/BLUEPRINT_5/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_5>);
#endif
#ifdef BLUEPRINT_6
#include STRINGIFY(blueprints/BLUEPRINT_6/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_6>);
#endif
#ifdef BLUEPRINT_7
#include STRINGIFY(blueprints/BLUEPRINT_7/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_7>);
#endif
#ifdef BLUEPRINT_8
#include STRINGIFY(blueprints/BLUEPRINT_8/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_8>);
#endif
#ifdef BLUEPRINT_9
#include STRINGIFY(blueprints/BLUEPRINT_9/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_9>);
#endif
#ifdef BLUEPRINT_10
#include STRINGIFY(blueprints/BLUEPRINT_10/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_10>);
#endif
#ifdef BLUEPRINT_11
#include STRINGIFY(blueprints/BLUEPRINT_11/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_11>);
#endif
#ifdef BLUEPRINT_12
#include STRINGIFY(blueprints/BLUEPRINT_12/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_12>);
#endif
#ifdef BLUEPRINT_13
#include STRINGIFY(blueprints/BLUEPRINT_13/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_13>);
#endif
#ifdef BLUEPRINT_14
#include STRINGIFY(blueprints/BLUEPRINT_14/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_14>);
#endif
#ifdef BLUEPRINT_15
#include STRINGIFY(blueprints/BLUEPRINT_15/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_15>);
#endif
#ifdef BLUEPRINT_16
#include STRINGIFY(blueprints/BLUEPRINT_16/blueprint.h)
STATIC_ASSERT(check_blueprint<BLUEPRINT_16>);
#endif

// #include hashmaps and check them for correctness.

#ifdef HASHMAP_1
#include STRINGIFY(hashmaps/HASHMAP_1/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_2
#include STRINGIFY(hashmaps/HASHMAP_2/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_3
#include STRINGIFY(hashmaps/HASHMAP_3/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_4
#include STRINGIFY(hashmaps/HASHMAP_4/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_5
#include STRINGIFY(hashmaps/HASHMAP_5/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_6
#include STRINGIFY(hashmaps/HASHMAP_6/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_7
#include STRINGIFY(hashmaps/HASHMAP_7/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_8
#include STRINGIFY(hashmaps/HASHMAP_8/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_9
#include STRINGIFY(hashmaps/HASHMAP_9/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_10
#include STRINGIFY(hashmaps/HASHMAP_10/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_11
#include STRINGIFY(hashmaps/HASHMAP_11/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_12
#include STRINGIFY(hashmaps/HASHMAP_12/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_13
#include STRINGIFY(hashmaps/HASHMAP_13/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_14
#include STRINGIFY(hashmaps/HASHMAP_14/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_15
#include STRINGIFY(hashmaps/HASHMAP_15/hashmap_wrapper.h)
#endif
#ifdef HASHMAP_16
#include STRINGIFY(hashmaps/HASHMAP_16/hashmap_wrapper.h)
#endif

// Benchmark ids.
enum benchmark_ids {
    id_find_existing,
    id_find_non_existing,
    id_insert_non_existing,
    id_insert_existing,    
    id_replace_existing,
    id_erase_existing,
    id_erase_non_existing,
    id_iteration
};

// Benchmark names used in the graphs.
const char * benchmark_names[] = {
    "Total time to look up 1,000 existing keys with N keys in the table",
    "Total time to look up 1,000 non-existing keys with N keys in the table",
    "Total time to insert N non-existing keys",
    "Total time to insert N existing keys",    
    "Total time to replace 1,000 existing keys with N keys in the table",
    "Total time to erase 1,000 existing keys with N keys in the table",
    "Total time to erase 1,000 non-existing keys with N keys in the table",
    "Total time to iterate over 5,000 keys with N keys in the table"
};

// Benchmark names used in the heatmap.
const char * benchmark_short_names[] = {
    "Look up existing",
    "Look up non-existing",
    "Insert non-existing",
    "Insert existing",
    "Replace existing",
    "Erase existing",
    "Erase non-existing",
    "Iterate"
};

const char * get_benchmark_id(std::size_t benchmark_id)
{
    switch (benchmark_id) {
        case id_find_existing:
            return "id_find_existing";
        case id_find_non_existing:
            return "id_find_non_existing";
        case id_insert_non_existing:
            return "id_insert_non_existing";
        case id_insert_existing:
            return "id_insert_existing";
        case id_replace_existing:
            return "id_replace_existing";
        case id_erase_existing:
            return "id_erase_existing";
        case id_erase_non_existing:
            return "id_erase_non_existing";
        case id_iteration:
            return "id_iteration";
        default:
            return "Unknown benchmark id";
    }
}

const char * get_benchmark_short_name(std::size_t benchmark_id)
{
    std::size_t max_id = jstd_countof(benchmark_short_names);
    if (benchmark_id < max_id)
        return benchmark_short_names[benchmark_id];
    else
        return "Unknown benchmark id";
}

namespace detail {

template <std::size_t N, std::size_t NextPow>
struct Pow10_impl {
    static constexpr const std::size_t value = 10 * Pow10_impl<N, NextPow - 1>::value;
};

template <std::size_t N>
struct Pow10_impl<N, 0> {
    static constexpr const std::size_t value = 1;
};

template <std::size_t N>
struct Pow10 {
    static constexpr const std::size_t value = Pow10_impl<N, N>::value;
};

template <std::size_t N>
void format_with_zeros(std::string & str, std::size_t value)
{
    std::size_t out[N];
    std::intptr_t digits = N - 1;
    while (value != 0) {
        std::size_t digit = value % 10;
        out[digits--] = digit;
        value /= 10;
    }
    std::intptr_t i = 0;
    digits++;
    // Fill leading zeros
    for (; i < digits; i++) {
        str.push_back('0');
    }
    // Fill digits
    for (; i < (std::intptr_t)N; i++) {
        str.push_back('0' + static_cast<char>(out[i]));
    }
}

template <std::size_t N>
std::string format_integer(std::size_t value)
{
    static constexpr const std::size_t base_N = detail::Pow10<N>::value;

    std::string str;
    while (value != 0) {
        std::size_t part = value % base_N;
        value /= base_N;
        std::string spart;
        if (value != 0) {
            spart = ",";
            format_with_zeros<N>(spart, part);
        } else {
            spart = std::to_string(part);
        }
        str = spart + str;
    }
    return str;
}

} // namespace detail

// Random number generator.
std::default_random_engine random_number_generator(20250118U);

//
// Function for providing unique keys for a given blueprint in random order.
// Besides the [data_size] keys to be inserted, it also provides an extra [data_size] * 2
// keys for testing failed look-ups.
//
template <typename BluePrint>
void shuffled_unique_key(std::vector<typename BluePrint::key_type> & keys, std::size_t data_size)
{
    keys.clear();
    keys.resize(data_size * KEY_SCALE);
    BluePrint::fill_unique_keys(keys);
    std::shuffle(keys.begin(), keys.end(), random_number_generator);
}

//
// Function for recording and accessing a single result, i.e. one measurement for a particular table, blueprint, and
// benchmark during a particular run.
//
template <template<typename> typename HashMap, typename BluePrint, benchmark_ids benchmark_id>
std::uint64_t & results(std::size_t run_index, std::size_t result_index)
{
    static auto results = std::vector<std::uint64_t>(RUN_COUNT * (KEY_COUNT / KEY_COUNT_MEASUREMENT_INTERVAL));
    return results[run_index * (KEY_COUNT / KEY_COUNT_MEASUREMENT_INTERVAL) + result_index];
}

//
// Function that attempts to reset the cache to the same state before each table is benchmarked.
// The strategy is to iterate over an array that is at least as large as the L1, L2, and L3 caches combined.
//
void flush_cache()
{
    static auto buffer = std::vector<std::uint64_t>(APPROXIMATE_CACHE_SIZE / sizeof(std::uint64_t) + 1);

    volatile std::size_t r = 0;
    for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
        r += *iter;
    }

    ::srand(static_cast<unsigned int>(r));
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_find_existing(std::size_t run,
                             std::vector<typename BluePrint::key_type> & keys,
                             double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_find_non_existing(std::size_t run,
                                 std::vector<typename BluePrint::key_type> & keys,
                                 double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_insert_non_existing(std::size_t run,
                                   std::vector<typename BluePrint::key_type> & keys,
                                   double & elapsed_time)
{
    auto table = HashMap<BluePrint>::create_table();
    std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECOND_COOLDOWN_BETWEEN_BENCHMARKS));

    jtest::StopWatch sw;

    sw.start();
    for (std::size_t i = 0; i < kDataSize; i++) {
        HashMap<BluePrint>::insert(table, keys[i]);
    }
    sw.stop();

    elapsed_time = sw.getElapsedMillisec();
    printf("elapsed time = %0.3f ms\n", elapsed_time);

    results<HashMap, BluePrint, id_insert_non_existing>(run, 0) =
        static_cast<std::uint64_t>(sw.getElapsedMicrosec() * (1000.0 * 1000.0));
    HashMap<BluePrint>::destroy_table(table);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_insert_existing(std::size_t run,
                               std::vector<typename BluePrint::key_type> & keys,
                               double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_replace_existing(std::size_t run,
                                std::vector<typename BluePrint::key_type> & keys,
                                double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_erase_existing(std::size_t run,
                              std::vector<typename BluePrint::key_type> & keys,
                              double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_erase_non_existing(std::size_t run,
                                  std::vector<typename BluePrint::key_type> & keys,
                                  double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_iteration(std::size_t run,
                         std::vector<typename BluePrint::key_type> & keys,
                         double & elapsed_time)
{
    elapsed_time = 0;
    //printf("elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint,
          std::size_t BenchmarkId, std::size_t kDataSize>
void run_benchmark(std::size_t run, std::vector<typename BluePrint::key_type> & keys)
{
    std::cout << "Run " << (run + 1) << ", "; // << std::endl;

    double elapsed_time;

    if (0) {
        // Do nothing !!
    } else if (BenchmarkId == id_find_existing) {
        benchmark_find_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_find_non_existing) {
        benchmark_find_non_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_insert_non_existing) {
        benchmark_insert_non_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_insert_existing) {
        benchmark_insert_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_replace_existing) {
        benchmark_replace_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_erase_existing) {
        benchmark_erase_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_erase_non_existing) {
        benchmark_erase_non_existing<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else if (BenchmarkId == id_iteration) {
        benchmark_iteration<HashMap, BluePrint, kDataSize>(run, keys, elapsed_time);
    } else {
        // Unknown benchmard id
        std::cout << "Unknown benchmark id: " << BenchmarkId << std::endl;
    }

    //std::cout << std::endl;
}

template <template <typename> typename HashMap, typename BluePrint,
          std::size_t BenchmarkId, std::size_t kDataSize>
void run_benchmark_loop(std::vector<typename BluePrint::key_type> & keys)
{
    using emlment_type = typename BluePrint::emlment_type;

    std::cout << "Benchmark Id: " << get_benchmark_id(BenchmarkId)
              << ", Data size: " << detail::format_integer<3>(kDataSize)
              << ", Emlment size: " << sizeof(emlment_type) << "Bytes" << std::endl;
    std::cout << HashMap<void>::name << ": " << BluePrint::name << std::endl;
    std::cout << std::endl;
    for (std::size_t run = 0; run < RUN_COUNT; ++run) {
        run_benchmark<HashMap, BluePrint, BenchmarkId, kDataSize>(run, keys);
    }
    std::cout << std::endl;
}

template <template <typename> typename HashMap, typename BluePrint>
void run_benchmarks()
{
    using key_type = typename BluePrint::key_type;

    static constexpr const std::size_t kDataSize = BluePrint::get_data_size();

    std::vector<key_type> unique_keys;
    shuffled_unique_key<BluePrint>(unique_keys, kDataSize);

#ifdef BENCHMARK_FIND_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_find_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_FIND_NON_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_find_non_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_INSERT_NON_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_insert_non_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_INSERT_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_insert_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_REPLACE_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_replace_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_ERASE_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_erase_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_ERASE_NON_EXISTING
    run_benchmark_loop<HashMap, BluePrint, id_erase_non_existing, kDataSize>(unique_keys);
#endif

#ifdef BENCHMARK_ITERATION
    run_benchmark_loop<HashMap, BluePrint, id_iteration, kDataSize>(unique_keys);
#endif
}

// Function for benchmarking a hashmap against all blueprints.
template <typename BluePrint>
void run_blueprint_benchmarks()
{
#ifdef HASHMAP_1
    run_benchmarks<HASHMAP_1, BluePrint>();
#endif
#ifdef HASHMAP_2
    run_benchmarks<HASHMAP_2, BluePrint>();
#endif
#ifdef HASHMAP_3
    run_benchmarks<HASHMAP_3, BluePrint>();
#endif
#ifdef HASHMAP_4
    run_benchmarks<HASHMAP_4, BluePrint>();
#endif
#ifdef HASHMAP_5
    run_benchmarks<HASHMAP_5, BluePrint>();
#endif
#ifdef HASHMAP_6
    run_benchmarks<HASHMAP_6, BluePrint>();
#endif
#ifdef HASHMAP_7
    run_benchmarks<HASHMAP_7, BluePrint>();
#endif
#ifdef HASHMAP_8
    run_benchmarks<HASHMAP_8, BluePrint>();
#endif
#ifdef HASHMAP_9
    run_benchmarks<HASHMAP_9, BluePrint>();
#endif
#ifdef HASHMAP_10
    run_benchmarks<HASHMAP_10, BluePrint>();
#endif
#ifdef HASHMAP_11
    run_benchmarks<HASHMAP_11, BluePrint>();
#endif
#ifdef HASHMAP_12
    run_benchmarks<HASHMAP_12, BluePrint>();
#endif
#ifdef HASHMAP_13
    run_benchmarks<HASHMAP_13, BluePrint>();
#endif
#ifdef HASHMAP_14
    run_benchmarks<HASHMAP_14, BluePrint>();
#endif
#ifdef HASHMAP_15
    run_benchmarks<HASHMAP_15, BluePrint>();
#endif
#ifdef HASHMAP_16
    run_benchmarks<HASHMAP_16, BluePrint>();
#endif
}

int main(int argc, char * argv[])
{
    // Get UTC time string with colons replaced by underscores.
    auto utc_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream ss;
    ss << std::put_time(::gmtime(&utc_time), "%Y-%m-%dT%H:%M:%S");
    auto time_str = ss.str();
    std::replace(time_str.begin(), time_str.end(), ':', '_');

#ifdef BLUEPRINT_1
    run_blueprint_benchmarks<BLUEPRINT_1>();
#endif
#ifdef BLUEPRINT_2
    run_blueprint_benchmarks<BLUEPRINT_2>();
#endif
#ifdef BLUEPRINT_3
    run_blueprint_benchmarks<BLUEPRINT_3>();
#endif
#ifdef BLUEPRINT_4
    run_blueprint_benchmarks<BLUEPRINT_4>();
#endif
#ifdef BLUEPRINT_5
    run_blueprint_benchmarks<BLUEPRINT_5>();
#endif
#ifdef BLUEPRINT_6
    run_blueprint_benchmarks<BLUEPRINT_6>();
#endif
#ifdef BLUEPRINT_7
    run_blueprint_benchmarks<BLUEPRINT_7>();
#endif
#ifdef BLUEPRINT_8
    run_blueprint_benchmarks<BLUEPRINT_8>();
#endif
#ifdef BLUEPRINT_9
    run_blueprint_benchmarks<BLUEPRINT_9>();
#endif
#ifdef BLUEPRINT_10
    run_blueprint_benchmarks<BLUEPRINT_10>();
#endif
#ifdef BLUEPRINT_11
    run_blueprint_benchmarks<BLUEPRINT_11>();
#endif
#ifdef BLUEPRINT_12
    run_blueprint_benchmarks<BLUEPRINT_12>();
#endif
#ifdef BLUEPRINT_13
    run_blueprint_benchmarks<BLUEPRINT_13>();
#endif
#ifdef BLUEPRINT_14
    run_blueprint_benchmarks<BLUEPRINT_14>();
#endif
#ifdef BLUEPRINT_15
    run_blueprint_benchmarks<BLUEPRINT_15>();
#endif
#ifdef BLUEPRINT_16
    run_blueprint_benchmarks<BLUEPRINT_16>();
#endif

    std::cout << "Outputting results\n";

    //html_out(time_str);

    //csv_out(time_str);

    std::cout << "Optimization preventer: " << do_not_optimize << "\n";
    std::cout << "Done\n";

#if defined(_MSC_VER) && defined(_DEBUG)
    jstd::Console::ReadKey();
#endif
}
