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
    id_iteration,
    Max_Benchmark_Id
};

// Benchmark names used in the heatmap.
const char * benchmark_names[] = {
    "Look up existing",
    "Look up non-existing",
    "Insert non-existing",
    "Insert existing",
    "Replace existing",
    "Erase existing",
    "Erase non-existing",
    "Iterate"
};

// Benchmark short names used in BenchmarkResult.h, the short name length must be 12 chars.
const char * benchmark_short_names[] = {
    " find.exist ",
    "  find.non  ",
    " insert.non ",
    "insert.exist",
    "   replace  ",
    " erase.exist",
    "  erase.non ",
    "  iteration "
};

// Benchmark names used in the graphs.
const char * benchmark_labels[] = {
    "Total time to look up 1,000 existing keys with N keys in the table",
    "Total time to look up 1,000 non-existing keys with N keys in the table",
    "Total time to insert N non-existing keys",
    "Total time to insert N existing keys",
    "Total time to replace 1,000 existing keys with N keys in the table",
    "Total time to erase 1,000 existing keys with N keys in the table",
    "Total time to erase 1,000 non-existing keys with N keys in the table",
    "Total time to iterate over 5,000 keys with N keys in the table"
};

const char * get_benchmark_id(std::size_t benchmark_id)
{
    switch (benchmark_id) {
        case id_find_existing:
            return "find_existing";
        case id_find_non_existing:
            return "find_non_existing";
        case id_insert_non_existing:
            return "insert_non_existing";
        case id_insert_existing:
            return "insert_existing";
        case id_replace_existing:
            return "replace_existing";
        case id_erase_existing:
            return "erase_existing";
        case id_erase_non_existing:
            return "erase_non_existing";
        case id_iteration:
            return "iteration";
        default:
            return "Unknown benchmark id";
    }
}

const char * get_benchmark_name(std::size_t benchmark_id)
{
    std::size_t max_id = jstd_countof(benchmark_names);
    if (benchmark_id < max_id)
        return benchmark_names[benchmark_id];
    else
        return "Unknown benchmark name";
}

// The short name length must be 12 chars.
const char * get_benchmark_short_name(std::size_t benchmark_id)
{
    std::size_t max_id = jstd_countof(benchmark_short_names);
    if (benchmark_id < max_id)
        return benchmark_short_names[benchmark_id];
    else
        return "Unknown name";
}

const char * get_benchmark_label(std::size_t benchmark_id)
{
    std::size_t max_id = jstd_countof(benchmark_labels);
    if (benchmark_id < max_id)
        return benchmark_labels[benchmark_id];
    else
        return "Unknown benchmark label";
}

//
// The value of benchmark_short_names[] needs to be used in the BenchmarkResult.h file.
//
#include "BenchmarkResult.h"

jtest::BenchmarkResults gBenchmarkResults;

namespace detail {

constexpr std::size_t round_div(std::size_t dividend, std::size_t divisor)
{
    // detail::round_div(): The divisor cannot be 0.
    return ((dividend + divisor - 1) / divisor);
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
// Remove the minimum and maximum time, and then take the average of the other values.
//
double calc_average_time(double elapsed_times[RUN_COUNT])
{
#if RUN_COUNT >= 3
    std::size_t minIndex = 0, maxIndex = 0;
    double minTime = elapsed_times[0];
    double maxTime = elapsed_times[0];
    for (std::size_t i = 1; i < RUN_COUNT; i++) {
        double elapsed_time = elapsed_times[i];
        if (elapsed_time < minTime) {
            minTime = elapsed_time;
            minIndex = i;
        }
        if (elapsed_time > maxTime) {
            maxTime = elapsed_time;
            maxIndex = i;
        }
    }

    std::size_t total_count;
    if (minIndex != maxIndex) {
        elapsed_times[minIndex] = 0.0;
        elapsed_times[maxIndex] = 0.0;
        total_count = RUN_COUNT - 2;
    } else {
        elapsed_times[minIndex] = 0.0;
        total_count = RUN_COUNT - 1;
    }

    double totol_time = 0.0;
    for (std::size_t i = 0; i < RUN_COUNT; i++) {
        totol_time += elapsed_times[i];
    }

    if (total_count != 0)
        return (totol_time / total_count);
    else
        return ((minTime + maxTime) / 2.0);
#else
    static constexpr const std::size_t total_count = RUN_COUNT;

    double totol_time = 0.0;
    for (std::size_t i = 0; i < total_count; i++) {
        totol_time += elapsed_times[i];
    }

    if (total_count > 0)
        return (totol_time / total_count);
    else
        return 0.0;
#endif // RUN_COUNT >= 3
}

//
// Function for recording and accessing a single result, i.e. one measurement for a particular table, blueprint, and
// benchmark during a particular run.
//
template <template<typename> typename HashMap, typename BluePrint, benchmark_ids benchmark_id>
double & results(std::size_t run_index, std::size_t result_index)
{
    static constexpr const std::size_t kDataSize = BluePrint::get_data_size();
    static constexpr const std::size_t kMeasurementInterval = KEY_COUNT_MEASUREMENT_INTERVAL;
    static constexpr const std::size_t kLoopTimes = detail::round_div(kDataSize, kMeasurementInterval) + 1;

    static auto results = std::vector<double>(RUN_COUNT * kLoopTimes);
    assert(run_index < RUN_COUNT);
    assert(result_index < kLoopTimes);
    return results[run_index * kLoopTimes + result_index];
}

//
// Function that attempts to reset the cache to the same state before each table is benchmarked.
// The strategy is to iterate over an array that is at least as large as the L1, L2, and L3 caches combined.
//
void flush_cache()
{
    static auto buffer = std::vector<std::uint64_t>(APPROXIMATE_CACHE_SIZE / sizeof(std::uint64_t) + 1);

    //volatile std::size_t r = 0;
    for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
        do_not_optimize += *iter;
    }

    //::srand(static_cast<unsigned int>(r));
}

void flush_cache_and_sleep()
{
    //
    // Reset the cache state.
    // This ensures that each table starts each benchmarking run on equal footing, but it does not prevent the cache
    // effects of one benchmark from potentially influencing latter benchmarks.
    //
    flush_cache();
    std::cout << "flush_cache(), sleep(" << MILLISECOND_COOLDOWN_BETWEEN_BENCHMARKS << "ms)";
    std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECOND_COOLDOWN_BETWEEN_BENCHMARKS));
    std::cout << "... ";
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_find_existing(std::size_t run,
                             std::vector<typename BluePrint::key_type> & keys,
                             double & elapsed_time)
{
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;
    std::size_t result_index = 0;

    std::size_t insert_begin = 0;
    while (insert_begin < kDataSize) {
        std::size_t insert_end = (std::min)(insert_begin + NUMS_INSERT_KEY, kDataSize);
        for (std::size_t i = insert_begin; i < insert_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        // To determine which keys to erase, we randomly chose a position in the sequence of keys already inserted and
        // then erase the subsequent 1000 keys, wrapping around to the start of the sequence if necessary.
        // This strategy has the potential drawback that keys are erased in the same order in which they were inserted.
        std::size_t insert_begin2 = std::uniform_int_distribution<std::size_t>
                                    (0, insert_end - NUMS_ERASE_KEY - 1)(random_number_generator);
        std::size_t insert_end2 = insert_begin2 + NUMS_ERASE_KEY;

        sw.start();
        for (std::size_t i = insert_begin2; i < insert_end2; i++) {
            auto iter = HashMap<BluePrint>::find(table, keys[i]);

            // Accessing the first byte of the value prevents the above call from being optimized out and ensures that
            // tables that can preform look-ups without accessing the values (because the keys are stored separately)
            // actually incur the additional cache miss that they would incur during normal use.
            do_not_optimize += *(unsigned char *)&HashMap<BluePrint>::get_value_from_iter(table, iter);
        }
        sw.stop();

        double used_time = sw.getElapsedMillisec();
        //printf("elapsed time = %0.3f ms\n", used_time);
        elapsed_time += used_time;

        results<HashMap, BluePrint, id_find_existing>(run, result_index) = used_time;

        result_index++;
        insert_begin = insert_end;
    }

    printf("Total elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_find_non_existing(std::size_t run,
                                 std::vector<typename BluePrint::key_type> & keys,
                                 double & elapsed_time)
{
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;
    std::size_t result_index = 0;

    std::size_t insert_begin = 0;
    while (insert_begin < kDataSize) {
        std::size_t insert_end = (std::min)(insert_begin + NUMS_INSERT_KEY, kDataSize);
        for (std::size_t i = insert_begin; i < insert_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        // To determine which nonexisting keys to attempt to erase, we randomly chose a position in the sequence of
        // nonexisting keys (which starts at KEY_COUNT) and then call erase for the subsequent 1000 keys, wrapping
        // around to the start of the sequence if necessary.
        std::size_t find_begin = std::uniform_int_distribution<std::size_t>
                                    (kDataSize, kDataSize + insert_end - NUMS_ERASE_KEY - 1)(random_number_generator);
        std::size_t find_end = find_begin + NUMS_ERASE_KEY;

        sw.start();
        for (std::size_t i = find_begin; i < find_end; i++) {
            auto iter = HashMap<BluePrint>::find(table, keys[i]);
            // Should always be false.
            do_not_optimize += HashMap<BluePrint>::is_iter_valid(table, iter);
        }
        sw.stop();

        double used_time = sw.getElapsedMillisec();
        //printf("elapsed time = %0.3f ms\n", used_time);
        elapsed_time += used_time;

        results<HashMap, BluePrint, id_find_non_existing>(run, result_index) = used_time;

        result_index++;
        insert_begin = insert_end;
    }

    printf("Total elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_insert_non_existing(std::size_t run,
                                   std::vector<typename BluePrint::key_type> & keys,
                                   double & elapsed_time)
{
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;

    sw.start();
    for (std::size_t i = 0; i < kDataSize; i++) {
        HashMap<BluePrint>::insert(table, keys[i]);
    }
    sw.stop();

    elapsed_time = sw.getElapsedMillisec();
    printf("elapsed time = %0.3f ms\n", elapsed_time);

    results<HashMap, BluePrint, id_insert_non_existing>(run, 0) = elapsed_time;
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_insert_existing(std::size_t run,
                               std::vector<typename BluePrint::key_type> & keys,
                               double & elapsed_time)
{
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;
    std::size_t result_index = 0;

    std::size_t insert_begin = 0;
    while (insert_begin < kDataSize) {
        std::size_t insert_end = (std::min)(insert_begin + NUMS_INSERT_KEY, kDataSize);
        for (std::size_t i = insert_begin; i < insert_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        // To determine which keys to erase, we randomly chose a position in the sequence of keys already inserted and
        // then erase the subsequent 1000 keys, wrapping around to the start of the sequence if necessary.
        // This strategy has the potential drawback that keys are erased in the same order in which they were inserted.
        std::size_t insert_begin2 = std::uniform_int_distribution<std::size_t>
                                    (0, insert_end - NUMS_ERASE_KEY - 1)(random_number_generator);
        std::size_t insert_end2 = insert_begin2 + NUMS_ERASE_KEY;

        sw.start();
        for (std::size_t i = insert_begin2; i < insert_end2; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }
        sw.stop();

        double used_time = sw.getElapsedMillisec();
        //printf("elapsed time = %0.3f ms\n", used_time);
        elapsed_time += used_time;

        results<HashMap, BluePrint, id_insert_existing>(run, result_index) = used_time;

        result_index++;
        insert_begin = insert_end;
    }

    printf("Total elapsed time = %0.3f ms\n", elapsed_time);
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
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;
    std::size_t result_index = 0;

    std::size_t insert_begin = 0;
    while (insert_begin < kDataSize) {
        std::size_t insert_end = (std::min)(insert_begin + NUMS_INSERT_KEY, kDataSize);
        for (std::size_t i = insert_begin; i < insert_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        // To determine which keys to erase, we randomly chose a position in the sequence of keys already inserted and
        // then erase the subsequent 1000 keys, wrapping around to the start of the sequence if necessary.
        // This strategy has the potential drawback that keys are erased in the same order in which they were inserted.
        std::size_t erase_begin = std::uniform_int_distribution<std::size_t>
                                    (0, insert_end - NUMS_ERASE_KEY - 1)(random_number_generator);
        std::size_t erase_end = erase_begin + NUMS_ERASE_KEY;

        sw.start();
        for (std::size_t i = erase_begin; i < erase_end; i++) {
            HashMap<BluePrint>::erase(table, keys[i]);
        }
        sw.stop();

        double used_time = sw.getElapsedMillisec();
        //printf("elapsed time = %0.3f ms\n", used_time);
        elapsed_time += used_time;

        results<HashMap, BluePrint, id_erase_existing>(run, result_index) = used_time;

        // Re-insert the erased keys.
        // This has the drawback that if tombstones are being used, those tombstones created by the above erasures will
        // all be replaced by the re-inserted keys.
        // Hence, this benchmark cannot show the lingering effect of tombstones.
        for (std::size_t i = erase_begin; i < erase_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        result_index++;
        insert_begin = insert_end;
    }

    printf("Total elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_erase_non_existing(std::size_t run,
                                  std::vector<typename BluePrint::key_type> & keys,
                                  double & elapsed_time)
{
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;
    std::size_t result_index = 0;

    std::size_t insert_begin = 0;
    while (insert_begin < kDataSize) {
        std::size_t insert_end = (std::min)(insert_begin + NUMS_INSERT_KEY, kDataSize);
        for (std::size_t i = insert_begin; i < insert_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        // To determine which nonexisting keys to attempt to erase, we randomly chose a position in the sequence of
        // nonexisting keys (which starts at KEY_COUNT) and then call erase for the subsequent 1000 keys, wrapping
        // around to the start of the sequence if necessary.
        std::size_t erase_begin = std::uniform_int_distribution<std::size_t>
                                    (kDataSize, kDataSize + insert_end - NUMS_ERASE_KEY - 1)(random_number_generator);
        std::size_t erase_end = erase_begin + NUMS_ERASE_KEY;

        sw.start();
        for (std::size_t i = erase_begin; i < erase_end; i++) {
            HashMap<BluePrint>::erase(table, keys[i]);
        }
        sw.stop();

        double used_time = sw.getElapsedMillisec();
        //printf("elapsed time = %0.3f ms\n", used_time);
        elapsed_time += used_time;

        results<HashMap, BluePrint, id_erase_non_existing>(run, result_index) = used_time;

        result_index++;
        insert_begin = insert_end;
    }

    printf("Total elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint, std::size_t kDataSize>
void benchmark_iteration(std::size_t run,
                         std::vector<typename BluePrint::key_type> & keys,
                         double & elapsed_time)
{
    flush_cache_and_sleep();

    using table_type = typename HashMap<BluePrint>::table_type;
    table_type table;

    jtest::StopWatch sw;
    std::size_t result_index = 0;

    std::size_t insert_begin = 0;
    while (insert_begin < kDataSize) {
        std::size_t insert_end = (std::min)(insert_begin + NUMS_INSERT_KEY, kDataSize);
        for (std::size_t i = insert_begin; i < insert_end; i++) {
            HashMap<BluePrint>::insert(table, keys[i]);
        }

        // To determine which keys to erase, we randomly chose a position in the sequence of keys already inserted and
        // then erase the subsequent 1000 keys, wrapping around to the start of the sequence if necessary.
        // This strategy has the potential drawback that keys are erased in the same order in which they were inserted.
        std::size_t find_begin = std::uniform_int_distribution<std::size_t>
                                    (0, insert_end - 1)(random_number_generator);
        std::size_t find_end = find_begin + NUMS_ERASE_KEY;

        // To determine where inside the table to begin iteration, we randomly choose an existing key.
        // This ensures that we are not just hitting the same, cached memory every time we measure.
        auto iter = HashMap<BluePrint>::find(table, keys[find_begin]);

        sw.start();
        for (std::size_t i = find_begin; i < find_end; i++) {
            // Accessing the first bytes of the key and value ensures that tables that iterate without directly accessing
            // the keys and/or values actually incur the additional cache misses that they would incur during normal
            // use.
            do_not_optimize += *(unsigned char *)&HashMap<BluePrint>::get_key_from_iter(table, iter);
            //do_not_optimize += *(unsigned char *)&HashMap<BluePrint>::get_value_from_iter(table, iter);

            HashMap<BluePrint>::increment_iter(table, iter);
            if (unlikely(!HashMap<BluePrint>::is_iter_valid(table, iter))) {
                iter = HashMap<BluePrint>::begin_iter(table);
            }
        }
        sw.stop();

        double used_time = sw.getElapsedMillisec();
        //printf("elapsed time = %0.3f ms\n", used_time);
        elapsed_time += used_time;

        results<HashMap, BluePrint, id_iteration>(run, result_index) = used_time;

        result_index++;
        insert_begin = insert_end;
    }

    printf("Total elapsed time = %0.3f ms\n", elapsed_time);
}

template <template <typename> typename HashMap, typename BluePrint,
          std::size_t BenchmarkId, std::size_t kDataSize>
void run_benchmark(std::size_t run, std::vector<typename BluePrint::key_type> & keys,
                   double & elapsed_time)
{
    std::cout << "Run " << (run + 1) << ", "; // << std::endl;

    elapsed_time = 0.0;

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
    using element_type = typename BluePrint::element_type;

    jtest::BenchmarkCategory * category = nullptr;
    std::string strBluePrintId;

    jtest::BenchmarkBluePrint * blueprint = gBenchmarkResults.getBluePrint(BluePrint::name);
    if (blueprint != nullptr) {
        strBluePrintId = " (";
        strBluePrintId += std::to_string(blueprint->id());
        strBluePrintId += ")";
        jtest::BenchmarkHashmap * hashmap = blueprint->getHashmap(HashMap<void>::name);
        if (hashmap != nullptr) {
            category = hashmap->addCategory(BenchmarkId, get_benchmark_name(BenchmarkId),
                                                         get_benchmark_label(BenchmarkId));
        }
    }

    std::cout << std::endl;
    std::cout << "BluePrint: " << BluePrint::name << strBluePrintId << ", "
              << "Data size: " << jtest::detail::format_integer<3>(kDataSize) << ", "
              << "Element size: " << sizeof(element_type) << " Bytes" << std::endl;
    std::cout << HashMap<void>::name << ", "
              << "Benchmark Id: " << get_benchmark_id(BenchmarkId)
              << std::endl;
    std::cout << std::endl;

    double elapsed_time = 0.0;
    double elapsed_times[RUN_COUNT] = { 0.0 };

    for (std::size_t run = 0; run < RUN_COUNT; run++) {
        run_benchmark<HashMap, BluePrint, BenchmarkId, kDataSize>(run, keys, elapsed_time);
        elapsed_times[run] = elapsed_time;
    }

    double average_time = calc_average_time(elapsed_times);
    printf("Average time = %0.3f ms\n", average_time);

    if (category != nullptr) {
        jtest::BenchmarkResult * result = category->addResult(HashMap<void>::name, BluePrint::name, BenchmarkId,
                                                              average_time, elapsed_times, 0);
        assert(result != nullptr);
    }

    //std::cout << std::endl;
}

template <template <typename> typename HashMap, typename BluePrint>
void run_benchmarks()
{
    using key_type = typename BluePrint::key_type;

    static constexpr const std::size_t kDataSize = BluePrint::get_data_size();

    jtest::BenchmarkBluePrint * blueprint = gBenchmarkResults.getBluePrint(BluePrint::name);
    if (blueprint != nullptr) {
        blueprint->addHashmap(HashMap<void>::name, HashMap<void>::label);
    }

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

    // Display results when a hashmap all benchmark id done.
    gBenchmarkResults.printResults();
}

// Function for benchmarking a hashmap against all blueprints.
template <typename BluePrint>
void run_blueprint_benchmarks()
{
    gBenchmarkResults.addBluePrint<BluePrint>(BluePrint::name, BluePrint::label);

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
    std::ostringstream startTime;
    startTime << std::put_time(::gmtime(&utc_time), "%Y-%m-%d %H:%M:%S");
    auto time_str = startTime.str();
    std::replace(time_str.begin(), time_str.end(), ':', '_');

    std::cout << std::endl;
    std::cout << "Start time: " << startTime.str() << std::endl;

    jtest::StopWatch sw;
    sw.start();

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

    sw.stop();

    utc_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream endTime;
    endTime << std::put_time(::gmtime(&utc_time), "%Y-%m-%d %H:%M:%S");
#if 0
    std::cout << std::endl;
    std::cout << "Outputting results." << std::endl;

    gBenchmarkResults.printResults();
#endif
    std::cout << "Start time : " << startTime.str() << std::endl;
    std::cout << "End time   : " << endTime.str() << std::endl;
    std::cout << std::endl;
    std::cout << "Total elapsed time: " << sw.getElapsedSecond() << " Seconds." << std::endl;
    std::cout << std::endl;

    //html_out(time_str);
    //csv_out(time_str);

    std::cout << "Optimization preventer: " << do_not_optimize << std::endl;
    std::cout << "Done." << std::endl << std::endl;

#if defined(_MSC_VER) && defined(_DEBUG)
    jstd::Console::ReadKey();
#endif
}
