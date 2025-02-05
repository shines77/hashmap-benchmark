
/************************************************************************************

  CC BY-SA 4.0 License

  Copyright (c) 2020-2025 XiongHui Guo (gz_shines at msn.com)

  https://github.com/shines77/jstd_hashmap
  https://gitee.com/shines77/jstd_hashmap

*************************************************************************************

  CC Attribution-ShareAlike 4.0 International

  https://creativecommons.org/licenses/by-sa/4.0/deed.en

  You are free to:

    1. Share -- copy and redistribute the material in any medium or format.

    2. Adapt -- remix, transforn, and build upon the material for any purpose,
    even commerically.

    The licensor cannot revoke these freedoms as long as you follow the license terms.

  Under the following terms:

    * Attribution -- You must give appropriate credit, provide a link to the license,
    and indicate if changes were made. You may do so in any reasonable manner,
    but not in any way that suggests the licensor endorses you or your use.

    * ShareAlike -- If you remix, transform, or build upon the material, you must
    distribute your contributions under the same license as the original.

    * No additional restrictions -- You may not apply legal terms or technological
    measures that legally restrict others from doing anything the license permits.

  Notices:

    * You do not have to comply with the license for elements of the material
    in the public domain or where your use is permitted by an applicable exception
    or limitation.

    * No warranties are given. The license may not give you all of the permissions
    necessary for your intended use. For example, other rights such as publicity,
    privacy, or moral rights may limit how you use the material.

************************************************************************************/

#ifdef _MSC_VER
#include <jstd/basic/vld.h>
#endif

#ifdef _MSC_VER
#ifndef __SSE4_2__
#define __SSE4_2__
#endif
#endif

// For avoid the MSVC stdext::hasp_map<K,V>'s deprecation warnings.
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include <jstd/basic/stddef.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cstddef>
#include <string>
#include <cstring>
#include <atomic>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

#define USE_STD_HASH_MAP                0
#define USE_STD_UNORDERED_MAP           1
#define USE_JSTD_ROBIN_HASH_MAP         1
#define USE_SKA_FLAT_HASH_MAP           0
#define USE_SKA_BYTELL_HASH_MAP         0
#define USE_EMHASH5_HASH_MAP            0
#define USE_EMHASH7_HASH_MAP            1
#define USE_EMHASH8_HASH_MAP            1
#define USE_ABSL_FLAT_HASH_MAP          1
#define USE_ABSL_NODE_HASH_MAP          0

#define USE_TSL_ROBIN_HOOD              0
#define USE_ROBIN_HOOD_UNORDERED_MAP    0
#define USE_ANKERL_UNORDERED_DENSE      1

#define USE_JSTD_GROUP16_FALT_MAP       1
#define USE_JSTD_GROUP15_FALT_MAP       1
#if (jstd_cplusplus >= 2020L)
// Need C++ 20
#define USE_BOOST_UNORDERED_FLAT_MAP    1
#endif

#ifdef _MSC_VER
#undef USE_ABSL_FLAT_HASH_MAP
#undef USE_ABSL_NODE_HASH_MAP
#undef USE_EMHASH7_HASH_MAP
#undef USE_EMHASH8_HASH_MAP

#undef USE_TSL_ROBIN_HOOD
#undef USE_ROBIN_HOOD_UNORDERED_MAP
#undef USE_ANKERL_UNORDERED_DENSE
#endif

#ifdef __SSE4_2__

// Support SSE 4.2: _mm_crc32_u32(), _mm_crc32_u64().
#define JSTD_HAVE_SSE42_CRC32C  1

// Support Intel SMID SHA module: sha1 & sha256, it's higher than SSE 4.2 .
// _mm_sha1msg1_epu32(), _mm_sha1msg2_epu32() and so on.
#define JSTD_HAVE_SMID_SHA      0

#endif // __SSE4_2__

// String compare mode
#define STRING_UTILS_STL        0
#define STRING_UTILS_U64        1
#define STRING_UTILS_SSE42      2

#define STRING_UTILS_MODE       STRING_UTILS_STL

#include <jstd/basic/stddef.h>
#include <jstd/basic/stdint.h>
#include <jstd/basic/inttypes.h>

#if USE_STD_HASH_MAP
#if defined(_MSC_VER)
#include <hash_map>
#define STDEXT_HASH_NAMESPACE stdext
#else
#include <ext/hash_map>
#define STDEXT_HASH_NAMESPACE __gnu_cxx
#endif
#endif // USE_STD_HASH_MAP

#include <string>
#include <unordered_set>
#if USE_STD_UNORDERED_MAP
#include <unordered_map>
#endif

#if USE_JSTD_ROBIN_HASH_MAP
#include <jstd/hashmap/robin_hash_map.h>
#endif
#if USE_SKA_FLAT_HASH_MAP
#include <flat_hash_map/flat_hash_map.hpp>
#endif
#if USE_SKA_BYTELL_HASH_MAP
#include <flat_hash_map/bytell_hash_map.hpp>
#endif
#if USE_EMHASH5_HASH_MAP
#include <emhash/hash_table5.hpp>
#endif
#if USE_EMHASH7_HASH_MAP
#include <emhash/hash_table7.hpp>
#endif
#if USE_EMHASH8_HASH_MAP
#include <emhash/hash_table8.hpp>
#endif
#if USE_ABSL_FLAT_HASH_MAP
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Woverflow"
#include <absl/container/internal/raw_hash_set.cc>
#include <absl/base/internal/raw_logging.cc>
#include <absl/hash/internal/hash.cc>
#include <absl/hash/internal/city.cc>
#include <absl/hash/internal/low_level_hash.cc>
#include <absl/container/flat_hash_map.h>
#pragma GCC diagnostic pop
#endif
#if USE_ABSL_NODE_HASH_MAP
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Woverflow"
#include <absl/container/internal/raw_hash_set.cc>
#include <absl/base/internal/raw_logging.cc>
#include <absl/hash/internal/hash.cc>
#include <absl/hash/internal/city.cc>
#include <absl/hash/internal/low_level_hash.cc>
#include <absl/container/node_hash_map.h>
#pragma GCC diagnostic pop
#endif
#if USE_TSL_ROBIN_HOOD
#include <tsl/robin_map.h>
#endif
#if USE_ROBIN_HOOD_UNORDERED_MAP
#include <robin_hood.h>
#endif
#if USE_ANKERL_UNORDERED_DENSE
#include <ankerl/unordered_dense.h>
#endif
#if USE_JSTD_GROUP16_FALT_MAP
#include <jstd/hashmap/group16_flat_map.hpp>
#endif
#if USE_JSTD_GROUP15_FALT_MAP
#include <jstd/hashmap/group15_flat_map.hpp>
#endif
#if USE_BOOST_UNORDERED_FLAT_MAP
#include <boost/unordered/unordered_flat_map.hpp>
#endif
#include <jstd/hashmap/hashmap_analyzer.h>
#include <jstd/hasher/hashes.h>
#include <jstd/hasher/fnv1a.h>
#include <jstd/hasher/hash_helper.h>
#include <jstd/string/string_view.h>
#include <jstd/string/string_view_array.h>
#include <jstd/system/Console.h>
#include <jstd/system/RandomGen.h>
#include <jstd/test/StopWatch.h>
#include <jstd/test/CPUWarmUp.h>
#include <jstd/test/ProcessMemInfo.h>
#include <jstd/test/ReadRss.h>

#define USE_STAT_COUNTER        0

#if USE_STAT_COUNTER
#define USE_CTOR_COUNTER        0
#endif

#define ID_STD_HASH             0   // std::hash<T>
#define ID_STDEXT_HASH          1   // stdext::hash_compare<T> or __gnu_cxx::hash<T>
#define ID_SIMPLE_HASH          2   // test::SimpleHash<T>
#define ID_INTEGAL_HASH         3   // test::IntegalHash<T>
#define ID_MUM_HASH             4   // test::MumHash<T>

#ifdef _MSC_VER
#define HASH_FUNCTION_ID        ID_SIMPLE_HASH
#else
#define HASH_FUNCTION_ID        ID_SIMPLE_HASH
#endif

#if (HASH_FUNCTION_ID == ID_STDEXT_HASH)
  #if defined(_MSC_VER)
    #define HASH_FUNCTION_ID    STDEXT_HASH_NAMESPACE::hash_compare
  #else
    #define HASH_FUNCTION_ID    STDEXT_HASH_NAMESPACE::hash
  #endif
#elif (HASH_FUNCTION_ID == ID_SIMPLE_HASH)
  #define HASH_MAP_FUNCTION     test::SimpleHash
#elif (HASH_FUNCTION_ID == ID_INTEGAL_HASH)
  #define HASH_MAP_FUNCTION     test::IntegalHash
#elif (HASH_FUNCTION_ID == ID_MUM_HASH)
  #define HASH_MAP_FUNCTION     test::MumHash
#else
  #define HASH_MAP_FUNCTION     std::hash
#endif // HASH_FUNCTION_ID

#define MACRO_TO_STRING(x)      #x
#define PRINT_MACRO(x)          MACRO_TO_STRING(x)
#define PRINT_MACRO_VAR(x)      #x " = " MACRO_TO_STRING(x)

#ifndef UINT64_High
#define UINT64_High(u64)        ((uint32_t)((uint64_t)u64 >> 32))
#endif

#ifndef UINT64_Low
#define UINT64_Low(u64)         ((uint32_t)((uint64_t)u64 & 0x00000000FFFFFFFFull))
#endif

static constexpr bool FLAGS_test_4_bytes = true;
static constexpr bool FLAGS_test_8_bytes = true;
static constexpr bool FLAGS_test_16_bytes = true;
static constexpr bool FLAGS_test_32_bytes = true;

static bool FLAGS_test_string_only = false;

#ifndef _DEBUG
static constexpr std::size_t kDefaultIters = 10000000;
#else
static constexpr std::size_t kDefaultIters = 10000;
#endif

static constexpr std::size_t kInitCapacity = 8;

// Returns the number of hashes that have been done since the last
// call to NumHashesSinceLastCall().  This is shared across all
// HashObject instances, which isn't super-OO, but avoids two issues:
// (1) making HashObject bigger than it ought to be (this is very
// important for our testing), and (2) having to pass around
// HashObject objects everywhere, which is annoying.
#if USE_STAT_COUNTER
static std::size_t g_num_hashes = 0;
static std::size_t g_num_copies = 0;
static std::size_t g_num_assigns = 0;
#if USE_CTOR_COUNTER
static std::size_t g_num_constructor = 0;
#endif
#endif

static inline
void reset_counter()
{
#if USE_STAT_COUNTER
    g_num_hashes = 0;
    g_num_copies = 0;
    g_num_assigns = 0;
#if USE_CTOR_COUNTER
    g_num_constructor = 0;
#endif
#endif // USE_STAT_COUNTER
}

static inline
std::size_t CurrentMemoryUsage()
{
    __COMPILER_BARRIER();
    std::size_t usedMemory = jtest::getCurrentRSS();
    __COMPILER_BARRIER();
    return usedMemory;
}

namespace std {

template <>
struct hash<jstd::string_view> {
    typedef jstd::string_view   argument_type;
    typedef std::uint32_t       result_type;

    typedef jstd::string_hash_helper<jstd::string_view,
                                     std::uint32_t, jstd::HashFunc_CRC32C> hasher;

    result_type operator () (const argument_type & key) const noexcept {
        return static_cast<result_type>(hasher::getHashCode(key));
    }
};

template <>
struct hash<jstd::wstring_view> {
    typedef jstd::wstring_view  argument_type;
    typedef std::uint32_t       result_type;

    typedef jstd::string_hash_helper<jstd::wstring_view,
                                     std::uint32_t, jstd::HashFunc_CRC32C> hasher;

    result_type operator () (const argument_type & key) const noexcept {
        return static_cast<result_type>(hasher::getHashCode(key));
    }
};

template <typename CharT, typename Traits>
struct hash<jstd::basic_string_view<CharT, Traits>> {
    typedef jstd::basic_string_view<CharT, Traits>  argument_type;
    typedef std::uint32_t                           result_type;

    typedef jstd::string_hash_helper<jstd::basic_string_view<CharT, Traits>,
                                     std::uint32_t, jstd::HashFunc_CRC32C> hasher;

    result_type operator () (const argument_type & key) const noexcept {
        return static_cast<result_type>(hasher::getHashCode(key));
    }
};

} // namespace std

namespace test {

template <typename T>
struct SimpleHash {
    typedef T           argument_type;
    typedef std::size_t result_type;

    template <typename Integer, typename std::enable_if<
                                (std::is_integral<Integer>::value &&
                                (sizeof(Integer) <= 8))>::type * = nullptr>
    result_type operator () (Integer value) const noexcept {
        result_type hash = static_cast<result_type>(value);
        return hash;
    }

    template <typename Argument, typename std::enable_if<
                                 (!std::is_integral<Argument>::value ||
                                  sizeof(Argument) > 8)>::type * = nullptr>
    result_type operator () (const Argument & value) const
        noexcept(noexcept(std::declval<std::hash<Argument>>()(value))) {
        std::hash<Argument> hasher;
        return static_cast<result_type>(hasher(value));
    }
};

template <typename T>
struct IntegalHash
{
    typedef T           argument_type;
    typedef std::size_t result_type;

    using is_avalanching = std::true_type;

    template <typename UInt32, typename std::enable_if<
                                (std::is_integral<UInt32>::value &&
                                (sizeof(UInt32) <= 4))>::type * = nullptr>
    result_type operator () (UInt32 value) const noexcept {
        result_type hash = (result_type)(((std::uint64_t)value * 2654435761ul) >> 12);
        return hash;
    }

    template <typename UInt64, typename std::enable_if<
                                (std::is_integral<UInt64>::value &&
                                (sizeof(UInt64) > 4 && sizeof(UInt64) <= 8))>::type * = nullptr>
    result_type operator () (UInt64 value) const noexcept {
        result_type hash = (result_type)(((std::uint64_t)value * 11400714818402800987ull) >> 28);
        return hash;
    }

    template <typename Argument, typename std::enable_if<
                                  (!std::is_integral<Argument>::value ||
                                  sizeof(Argument) > 8)>::type * = nullptr>
    result_type operator () (const Argument & value) const
        noexcept(noexcept(std::declval<std::hash<Argument>>()(value))) {
        std::hash<Argument> hasher;
        return static_cast<result_type>(hasher(value));
    }
};

template <typename T>
struct MumHash
{
    typedef T           argument_type;
    typedef std::size_t result_type;

    using is_avalanching = std::true_type;

    template <typename Integer, typename std::enable_if<
                                (std::is_integral<Integer>::value &&
                                (sizeof(Integer) <= 8))>::type * = nullptr>
    result_type operator () (Integer value) const noexcept {
        result_type hash = (result_type)(jstd::hashes::mum_hash((std::size_t)value));
        return hash;
    }

    template <typename Argument, typename std::enable_if<
                                  (!std::is_integral<Argument>::value ||
                                  sizeof(Argument) > 8)>::type * = nullptr>
    result_type operator () (const Argument & value) const
        noexcept(noexcept(std::declval<std::hash<Argument>>()(value))) {
        std::hash<Argument> hasher;
        return static_cast<result_type>(hasher(value));
    }
};

} // namespace test

#if USE_STD_HASH_MAP

#if defined(_MSC_VER)

template <typename Key, typename Value, typename Hasher>
class StdHashMap : public STDEXT_HASH_NAMESPACE::hash_map<Key, Value, Hasher> {
public:
    typedef STDEXT_HASH_NAMESPACE::hash_map<Key, Value, Hasher> this_type;

    StdHashMap() : this_type() {}
    StdHashMap(std::size_t initCapacity) : this_type() {
    }

    void resize(size_t newSize) {
        /* Not support */
    }

    void rehash(std::size_t newSize) {
        this->resize(newSize);
    }
};

#else

template <typename T>
struct get_ident_type {
    typedef typename T::key_type ident_type;
};

template <typename T>
struct get_ident_type<T *> {
    typedef T * ident_type;
};

template <typename Key, typename Value, typename Hasher>
class StdHashMap : public STDEXT_HASH_NAMESPACE::hash_map<Key, Value, Hasher> {
public:
    typedef STDEXT_HASH_NAMESPACE::hash_map<Key, Value, Hasher> this_type;
    typedef Key                                         key_type;
    typedef Value                                       mapped_type;
    typedef typename get_ident_type<Key>::ident_type    ident_type;

    StdHashMap() : this_type() {}
    StdHashMap(std::size_t initCapacity) : this_type() {
    }

    void emplace(const key_type & key, const mapped_type & value) {
        this->operator [](key) = value;
    }

    void emplace(const key_type & key, mapped_type && value) {
        this->operator [](key) = std::forward<mapped_type>(value);
    }

    template <typename IdentType, typename std::enable_if<
                                                !std::is_same<key_type, IdentType>::value
                                            >::type * = nullptr>
    void emplace(const IdentType & id, const mapped_type & value) {
        key_type key(id);
        this->operator [](key) = value;
    }

    template <typename IdentType, typename std::enable_if<
                                                !std::is_same<key_type, IdentType>::value
                                            >::type * = nullptr>
    void emplace(const IdentType & id, mapped_type && value) {
        key_type key(id);
        this->operator [](key) = std::forward<mapped_type>(value);
    }

    void rehash(std::size_t newSize) {
        this->resize(newSize);
    }

    // Don't need to do anything: hash_map is already easy to use!
};

#endif // _MSC_VER

#endif // USE_STD_HASH_MAP

#if USE_STD_UNORDERED_MAP

template <typename Key, typename Value, typename Hasher>
class StdUnorderedMap : public std::unordered_map<Key, Value, Hasher> {
public:
    typedef std::unordered_map<Key, Value, Hasher> this_type;

    StdUnorderedMap() : this_type() {}
    StdUnorderedMap(std::size_t initCapacity) : this_type(initCapacity) {
    }

    // resize() is called rehash() in tr1
    void resize(std::size_t newSize) {
        this->rehash(newSize);
    }
};

#endif // USE_STD_UNORDERED_MAP

template <typename Container>
void print_test_time(std::size_t checksum, double elapsedTime)
{
    // printf("---------------------------------------------------------------------------\n");
    std::string name;
    if (jstd::has_static_name<Container>::value)
        name = jstd::call_static_name<Container>::name() + "<K, V>";
    else
        name = "std::unordered_map<K, V>";
    printf(" %-36s  sum = %-10" PRIuPTR "  time: %8.3f ms\n", name.c_str(), checksum, elapsedTime);
}

static void report_result(const char * title, double ut, double lf, std::size_t iters,
                          size_t start_memory, size_t end_memory, const char * info) {
    // Construct heap growth report text if applicable
    char heap[128];
    heap[0] = '\0';
#if 0
    if (end_memory > start_memory) {
        snprintf(heap, sizeof(heap), "%7.1f MB", (double)(end_memory - start_memory) / 1048576.0);
    }
#else
    if (end_memory > start_memory) {
        snprintf(heap, sizeof(heap), "%7.1f MB (+%0.1f MB)",
                 (double)start_memory / 1048576.0,
                 (double)(end_memory - start_memory) / 1048576.0);
    } else if (start_memory > end_memory) {
        snprintf(heap, sizeof(heap), "%7.1f MB (-%0.1f MB)",
                 (double)start_memory / 1048576.0,
                 (double)(start_memory - end_memory) / 1048576.0);
    } else if ((start_memory == end_memory) && (start_memory != 0)) {
        snprintf(heap, sizeof(heap), "%7.1f MB", (double)start_memory / 1048576.0);
    }
#endif

#if (USE_STAT_COUNTER == 0)
    printf("%-32s %8.2f ns  lf=%0.3f  %s  %s\n", title, (ut * 1000000000.0 / iters), lf, heap, info);
#else
  #if USE_CTOR_COUNTER
    printf("%-32s %8.2f ns  (%8" PRIuPTR " hashes, %8" PRIuPTR " copies, %8" PRIuPTR " assigns, %8" PRIuPTR " ctor)  lf=%0.3f  %s  %s\n",
           title, (ut * 1000000000.0 / iters),
           g_num_hashes, g_num_copies, g_num_assigns, g_num_constructor,
           lf, heap, info);
  #else
    printf("%-32s %8.2f ns  (%8" PRIuPTR " hashes, %8" PRIuPTR " copies, %8" PRIuPTR " assigns)  lf=%0.3f  %s  %s\n",
           title, (ut * 1000000000.0 / iters),
           g_num_hashes, g_num_copies, g_num_assigns,
           lf, heap, info);
  #endif
#endif
    ::fflush(stdout);
}

// Apply a pseudorandom permutation to the given vector.
template <typename Vector>
void shuffle_vector(Vector & vector, int seed = 0) {
    // shuffle
    if (seed == 0)
        seed = 20200831;
    jstd::MtRandomGen mtRandomGen(seed);
    for (std::size_t n = vector.size(); n >= 2; n--) {
        std::size_t rnd_idx = std::size_t(jstd::MtRandomGen::nextUInt()) % n;
        std::swap(vector[n - 1], vector[rnd_idx]);
    }
}

/* The implementation of test routine */
#include "time_hash_map_func.hpp"

/* The implementation of test routine */
#include "time_hash_map_object_func.hpp"

template <class MapType, class StressMapType>
void measure_hashmap(const char * name, std::size_t obj_size,
                     std::size_t iters, bool is_stress_hash_function)
{
    typedef typename MapType::value_type    value_type;
    typedef typename MapType::mapped_type   mapped_type;

    printf("%s<K, V> (%" PRIuPTR " byte objects, %" PRIuPTR " byte ValueType, %" PRIuPTR " iterations):\n",
           name, obj_size, sizeof(value_type), iters);

    if (1) printf("\n");

    //------------------------------------------------------------
    std::vector<mapped_type> rndIndices;
    rndIndices.reserve(iters);
    for (mapped_type i = 0; i < iters; i++) {
        rndIndices.push_back(i);
    }
    // Seed = 20220714
    shuffle_vector(rndIndices, 20220714);

    //------------------------------------------------------------

    if (1) map_serial_find_success<MapType>(iters);
    if (1) map_serial_find_random<MapType>(iters);
    if (1) map_serial_find_failed<MapType>(iters);
    if (1) map_serial_find_empty<MapType>(iters);
    if (1) printf("\n");

    if (1) map_random_find_serial<MapType>(iters, rndIndices);
    if (1) map_random_find_random<MapType>(iters, rndIndices);
    if (1) map_random_find_failed<MapType>(iters, rndIndices);
    if (1) map_random_find_empty<MapType>(iters, rndIndices);
    if (1) printf("\n");

    //------------------------------------------------------------

    if (1) map_serial_insert<MapType>(iters);
    if (1) map_serial_insert_predicted<MapType>(iters);
    if (1) map_serial_insert_replace<MapType>(iters);
    if (1) printf("\n");

    if (1) map_serial_emplace<MapType>(iters);
    if (1) map_serial_emplace_predicted<MapType>(iters);
    if (1) map_serial_emplace_replace<MapType>(iters);
    if (1) printf("\n");

    if (1) map_serial_operator<MapType>(iters);
    if (1) map_serial_operator_predicted<MapType>(iters);
    if (1) map_serial_operator_replace<MapType>(iters);
    if (1) printf("\n");

    //------------------------------------------------------------

    if (1) map_random_insert<MapType>(iters, rndIndices);
    if (1) map_random_insert_predicted<MapType>(iters, rndIndices);
    if (1) map_random_insert_replace<MapType>(iters, rndIndices);
    if (1) printf("\n");

    if (1) map_random_emplace<MapType>(iters, rndIndices);
    if (1) map_random_emplace_predicted<MapType>(iters, rndIndices);
    if (1) map_random_emplace_replace<MapType>(iters, rndIndices);
    if (1) printf("\n");

    if (1) map_random_operator<MapType>(iters, rndIndices);
    if (1) map_random_operator_predicted<MapType>(iters, rndIndices);
    if (1) map_random_operator_replace<MapType>(iters, rndIndices);
    if (1) printf("\n");

    //------------------------------------------------------------

    if (1) map_serial_erase<MapType>(iters);
    if (1) map_serial_erase_failed<MapType>(iters);
    if (1) map_serial_toggle<MapType>(iters);
    if (1) map_serial_iterate<MapType>(iters);
    if (1) printf("\n");

    if (1) map_random_erase<MapType>(iters, rndIndices);
    if (1) map_random_erase_failed<MapType>(iters, rndIndices);
    if (1) map_random_toggle<MapType>(iters, rndIndices);
    if (1) map_random_iterate<MapType>(iters, rndIndices);
    if (1) printf("\n");

    //------------------------------------------------------------

#ifndef _DEBUG
    // This last test is useful only if the map type uses hashing.
    // And it's slow, so use fewer iterations.
    if (is_stress_hash_function) {
        // Blank line in the output makes clear that what follows isn't part of the
        // table of results that we just printed.
        stress_hash_function<StressMapType>(iters / 4);
        printf("\n");
    }
#endif
}

template <typename StringType, std::size_t minLen, std::size_t maxLen>
StringType generate_random_string(jstd::MtRandomGen & randomGen)
{
    typedef StringType                      string_type;
    typedef typename StringType::value_type char_type;

    static constexpr std::size_t kMinAscii = 32;
    static constexpr std::size_t kMaxAscii = 126;
    static const char_type IdentChars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
    static const char_type BodyChars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_+-=*\\/ [](){}<>#$%&@!~|,;:?'`";
    static const std::size_t kMaxIdentChars = sizeof(IdentChars) - 1;
    static const std::size_t kMaxBodyChars = sizeof(BodyChars) - 1;

    string_type str;
    char_type rndChar;
    std::size_t length = minLen + (randomGen.nextInt() % std::size_t(maxLen - minLen));
    str.reserve(length);

    // First char: Ident
    rndChar = IdentChars[randomGen.nextInt() % kMaxIdentChars];
    str.push_back(rndChar);
    for (std::size_t i = 0; i < length - 1; i++) {
        rndChar = BodyChars[randomGen.nextInt() % kMaxBodyChars];
        str.push_back(rndChar);
    }
    if (rndChar == ' ') {
        rndChar = IdentChars[randomGen.nextInt() % kMaxIdentChars];
        str.pop_back();
        str.push_back(rndChar);
    }
    assert(std::strlen(str.c_str()) >= minLen);
    return str;
}

template <typename ValueType, typename Key, typename Value,
          std::size_t minKeyLen = 4, std::size_t maxKeyLen = 31,
          std::size_t minValueLen = 1, std::size_t maxValueLen = 31>
void generate_random_strings(std::size_t length, std::vector<ValueType> & kvs,
                            std::vector<Key> & keys,
                            std::vector<Key> & rnd_keys,
                            std::vector<Key> & miss_keys)
{
    jstd::MtRandomGen mtRandomGen(20220714);
    std::unordered_set<Key> key_used;

    kvs.reserve(length);
    keys.reserve(length);

    for (std::size_t i = 0; i < length; i++) {
        std::string key;
        do {
            key = generate_random_string<Key, minKeyLen, maxKeyLen>(mtRandomGen);
        } while (key_used.count(key) != 0);
        key_used.insert(key);

        std::string value = generate_random_string<Value, minValueLen, maxValueLen>(mtRandomGen);

        kvs.emplace_back(std::make_pair(key, value));
        keys.emplace_back(key);
    }

    std::unordered_set<Key> miss_key_used;
    miss_keys.reserve(length);

    for (std::size_t i = 0; i < length; i++) {
        std::string miss_key;
        do {
            miss_key = generate_random_string<Key, minKeyLen, maxKeyLen>(mtRandomGen);
        } while (key_used.count(miss_key) != 0 || miss_key_used.count(miss_key) != 0);
        miss_key_used.insert(miss_key);
        miss_keys.emplace_back(miss_key);
    }

    rnd_keys = keys;
    shuffle_vector(rnd_keys, 20220714);
}

template <typename MapType>
void measure_string_hashmap(const char * name, std::size_t obj_size, std::size_t iters)
{
    typedef typename MapType::value_type    value_type;
    typedef typename MapType::key_type      key_type;
    typedef typename MapType::mapped_type   mapped_type;

    printf("%s<K, V> (%" PRIuPTR " byte objects, %" PRIuPTR " byte ValueType, %" PRIuPTR " iterations):\n",
           name, obj_size, sizeof(value_type), iters);

    if (1) printf("\n");

    //------------------------------------------------------------
    typedef std::vector<value_type> PairVector;
    typedef std::vector<key_type>   KeyVector;

    PairVector kvs;
    KeyVector keys, rnd_keys, miss_keys;

    generate_random_strings<value_type, key_type, mapped_type, 4u, 31u, 1u, 31u>(
        iters, kvs, keys, rnd_keys, miss_keys);

    //------------------------------------------------------------

    if (1) map_serial_find_success<MapType, PairVector, KeyVector>(iters, kvs, keys);
    if (1) map_random_find_success<MapType, PairVector, KeyVector>(iters, kvs, rnd_keys);
    if (1) map_find_failed<MapType, PairVector, KeyVector>(iters, kvs, miss_keys);
    if (1) map_find_empty<MapType, PairVector, KeyVector>(iters, kvs, keys);
    if (1) printf("\n");

    //------------------------------------------------------------

    if (1) map_insert<MapType, PairVector>(iters, kvs);
    if (1) map_insert_predicted<MapType, PairVector>(iters, kvs);
    if (1) map_insert_replace<MapType, PairVector>(iters, kvs);
    if (1) printf("\n");

    if (1) map_emplace<MapType, PairVector>(iters, kvs);
    if (1) map_emplace_predicted<MapType, PairVector>(iters, kvs);
    if (1) map_emplace_replace<MapType, PairVector>(iters, kvs);
    if (1) printf("\n");

    if (1) map_try_emplace<MapType, PairVector>(iters, kvs);
    if (1) map_try_emplace_predicted<MapType, PairVector>(iters, kvs);
    if (1) map_try_emplace_replace<MapType, PairVector, KeyVector>(iters, kvs, rnd_keys);
    if (1) printf("\n");

    if (1) map_operator<MapType, PairVector>(iters, kvs);
    if (1) map_operator_predicted<MapType, PairVector>(iters, kvs);
    if (1) map_operator_replace<MapType, PairVector>(iters, kvs);
    if (1) printf("\n");

    //------------------------------------------------------------

    if (1) map_serial_erase<MapType, PairVector>(iters, kvs);
    if (1) map_random_erase<MapType, PairVector, KeyVector>(iters, kvs, rnd_keys);
    if (1) map_erase_failed<MapType, PairVector, KeyVector>(iters, kvs, miss_keys);
    if (1) printf("\n");

    if (1) map_toggle<MapType, PairVector>(iters, kvs);
    if (1) map_iterate<MapType, PairVector>(iters, kvs);
    if (1) printf("\n");
}

template <typename Key, typename Value>
void test_hashmap_by_name(const std::string & name, std::size_t obj_size, std::size_t iters)
{
    const bool has_stress_hash_function = (obj_size <= 8);

#if USE_STD_HASH_MAP
    if (name == "stdext::hash_map") {
        measure_hashmap<StdHashMap<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        StdHashMap<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("stdext::hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_STD_UNORDERED_MAP
    if (name == "std::unordered_map") {
        measure_hashmap<std::unordered_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        std::unordered_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("std::unordered_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_SKA_FLAT_HASH_MAP
    if (name == "ska::flat_hash_map") {
        measure_hashmap<ska::flat_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        ska::flat_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("ska::flat_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_SKA_BYTELL_HASH_MAP
    if (FLAGS_test_ska_bytell_hash_map && (name == "ska::bytell_hash_map")) {
        measure_hashmap<ska::bytell_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        ska::bytell_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("ska::bytell_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_TSL_ROBIN_HOOD
    if (name == "tsl::robin_map") {
        measure_hashmap<tsl::robin_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        tsl::robin_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("tsl::robin_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_ROBIN_HASH_MAP
    if (name == "jstd::robin_hash_map") {
        measure_hashmap<jstd::robin_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        jstd::robin_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("jstd::robin_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_EMHASH5_HASH_MAP
    if (name == "emhash5::HashMap") {
        measure_hashmap<emhash5::HashMap<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        emhash5::HashMap<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("emhash5::HashMap", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_EMHASH7_HASH_MAP
    if (name == "emhash7::HashMap") {
        measure_hashmap<emhash7::HashMap<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        emhash7::HashMap<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("emhash7::HashMap", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ROBIN_HOOD_UNORDERED_MAP
    if (name == "robin_hood::unordered_map") {
        measure_hashmap<robin_hood::unordered_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        robin_hood::unordered_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("robin_hood::unordered_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ANKERL_UNORDERED_DENSE
    if (name == "ankerl::unordered_dense") {
        measure_hashmap<ankerl::unordered_dense::map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        ankerl::unordered_dense::map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("ankerl::unordered_dense::map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ABSL_FLAT_HASH_MAP
    if (name == "absl::flat_hash_map") {
        measure_hashmap<absl::flat_hash_map<Key,   Value, test::MumHash<Key>>,
                        absl::flat_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("absl::flat_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ABSL_NODE_HASH_MAP
    if (name == "absl::node_hash_map") {
        measure_hashmap<absl::node_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        absl::node_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("absl::node_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_GROUP16_FALT_MAP
    if (name == "jstd::group16_flat_map") {
        measure_hashmap<jstd::group16_flat_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        jstd::group16_flat_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("jstd::group16_flat_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_GROUP15_FALT_MAP
    if (name == "jstd::group15_flat_map") {
        measure_hashmap<jstd::group15_flat_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        jstd::group15_flat_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("jstd::group15_flat_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_BOOST_UNORDERED_FLAT_MAP
    if (name == "boost::unordered_flat_map") {
        measure_hashmap<boost::unordered::unordered_flat_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        boost::unordered::unordered_flat_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("boost::unordered_flat_map", obj_size, iters, has_stress_hash_function);
    }
#endif
}

template <typename Key, typename Value>
void test_all_hashmaps(std::size_t obj_size, std::size_t iters)
{
    const bool has_stress_hash_function = (obj_size <= 8);

#if USE_STD_HASH_MAP
    if (1) {
        measure_hashmap<StdHashMap<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        StdHashMap<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("stdext::hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_STD_UNORDERED_MAP
    if (1) {
        measure_hashmap<std::unordered_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        std::unordered_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("std::unordered_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_SKA_FLAT_HASH_MAP
    if (1) {
        measure_hashmap<ska::flat_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        ska::flat_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("ska::flat_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_SKA_BYTELL_HASH_MAP
    if (FLAGS_test_ska_bytell_hash_map) {
        measure_hashmap<ska::bytell_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        ska::bytell_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("ska::bytell_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_TSL_ROBIN_HOOD
    if (1) {
        measure_hashmap<tsl::robin_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        tsl::robin_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("tsl::robin_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_ROBIN_HASH_MAP
    if (1) {
        measure_hashmap<jstd::robin_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        jstd::robin_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("jstd::robin_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_EMHASH5_HASH_MAP
    if (1) {
        measure_hashmap<emhash5::HashMap<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        emhash5::HashMap<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("emhash5::HashMap", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_EMHASH7_HASH_MAP
    if (1) {
        measure_hashmap<emhash7::HashMap<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        emhash7::HashMap<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("emhash7::HashMap", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ROBIN_HOOD_UNORDERED_MAP
    if (1) {
        measure_hashmap<robin_hood::unordered_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        robin_hood::unordered_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("robin_hood::unordered_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ANKERL_UNORDERED_DENSE
    if (1) {
        measure_hashmap<ankerl::unordered_dense::map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        ankerl::unordered_dense::map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("ankerl::unordered_dense::map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ABSL_FLAT_HASH_MAP
    if (1) {
        measure_hashmap<absl::flat_hash_map<Key,   Value, test::MumHash<Key>>,
                        absl::flat_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("absl::flat_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_ABSL_NODE_HASH_MAP
    if (1) {
        measure_hashmap<absl::node_hash_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        absl::node_hash_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("absl::node_hash_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_GROUP16_FALT_MAP
    if (1) {
        measure_hashmap<jstd::group16_flat_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        jstd::group16_flat_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("jstd::group16_flat_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_GROUP15_FALT_MAP
    if (1) {
        measure_hashmap<jstd::group15_flat_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        jstd::group15_flat_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("jstd::group15_flat_map", obj_size, iters, has_stress_hash_function);
    }
#endif

#if USE_BOOST_UNORDERED_FLAT_MAP
    if (1) {
        measure_hashmap<boost::unordered::unordered_flat_map<Key,   Value, HASH_MAP_FUNCTION<Key>>,
                        boost::unordered::unordered_flat_map<Key *, Value, HASH_MAP_FUNCTION<Key *>>>
            ("boost::unordered_flat_map", obj_size, iters, has_stress_hash_function);
    }
#endif
}

template <typename Key, typename Value>
void test_hashmap_by_name_for_string(const std::string & name, std::size_t obj_size, std::size_t iters)
{
#if USE_STD_HASH_MAP
    if (name == "stdext::hash_map") {
        measure_string_hashmap<StdHashMap<Key, Value>>
            ("stdext::hash_map", obj_size, iters);
    }
#endif

#if USE_STD_UNORDERED_MAP
    if (name == "std::unordered_map") {
        measure_string_hashmap<std::unordered_map<Key, Value>>
            ("std::unordered_map", obj_size, iters);
    }
#endif

#if USE_SKA_BYTELL_HASH_MAP
    if (name == "ska::bytell_hash_map") {
        measure_string_hashmap<ska::bytell_hash_map<Key, Value>>
            ("ska::bytell_hash_map", obj_size, iters);
    }
#endif

#if USE_SKA_FLAT_HASH_MAP
    if (name == "ska::flat_hash_map") {
        measure_string_hashmap<ska::flat_hash_map<Key, Value>>
            ("ska::flat_hash_map", obj_size, iters);
    }
#endif

#if USE_TSL_ROBIN_HOOD
    if (name == "tsl::robin_map") {
        measure_string_hashmap<tsl::robin_map<Key, Value>>
            ("tsl::robin_map", obj_size, iters);
    }
#endif

#if USE_JSTD_ROBIN_HASH_MAP
    if (name == "jstd::robin_hash_map") {
        measure_string_hashmap<jstd::robin_hash_map<Key, Value>>
            ("jstd::robin_hash_map", obj_size, iters);
    }
#endif

#if USE_EMHASH5_HASH_MAP
    if (name == "emhash5::HashMap") {
        measure_string_hashmap<emhash5::HashMap<Key, Value>>
            ("emhash5::HashMap", obj_size, iters);
    }
#endif

#if USE_EMHASH7_HASH_MAP
    if (name == "emhash7::HashMap") {
        measure_string_hashmap<emhash7::HashMap<Key, Value>>
            ("emhash7::HashMap", obj_size, iters);
    }
#endif

#if USE_EMHASH8_HASH_MAP
    if (name == "emhash8::HashMap") {
        measure_string_hashmap<emhash8::HashMap<Key, Value>>
            ("emhash8::HashMap", obj_size, iters);
    }
#endif

#if USE_ROBIN_HOOD_UNORDERED_MAP
    if (name == "robin_hood::unordered_map") {
        measure_string_hashmap<robin_hood::unordered_map<Key, Value>>
            ("robin_hood::unordered_map", obj_size, iters);
    }
#endif

#if USE_ANKERL_UNORDERED_DENSE
    if (name == "ankerl::unordered_dense::map") {
        measure_string_hashmap<ankerl::unordered_dense::map<Key, Value>>
            ("ankerl::unordered_dense::map", obj_size, iters);
    }
#endif

#if USE_ABSL_FLAT_HASH_MAP
    if (name == "absl::flat_hash_map") {
        measure_string_hashmap<absl::flat_hash_map<Key, Value>>
            ("absl::flat_hash_map", obj_size, iters);
    }
#endif

#if USE_ABSL_NODE_HASH_MAP
    if (name == "absl::node_hash_map") {
        measure_string_hashmap<absl::node_hash_map<Key, Value>>
            ("absl::node_hash_map", obj_size, iters);
    }
#endif

#if USE_JSTD_GROUP16_FALT_MAP
    if (name == "jstd::group16_flat_map") {
        measure_string_hashmap<jstd::group16_flat_map<Key, Value>>
            ("jstd::group16_flat_map", obj_size, iters);
    }
#endif

#if USE_JSTD_GROUP15_FALT_MAP
    if (name == "jstd::group15_flat_map") {
        measure_string_hashmap<jstd::group15_flat_map<Key, Value>>
            ("jstd::group15_flat_map", obj_size, iters);
    }
#endif

#if USE_BOOST_UNORDERED_FLAT_MAP
    if (name == "boost::unordered_flat_map") {
        measure_string_hashmap<boost::unordered::unordered_flat_map<Key, Value>>
            ("boost::unordered_flat_map", obj_size, iters);
    }
#endif
}

template <typename Key, typename Value>
void test_all_hashmaps_for_string(std::size_t obj_size, std::size_t iters)
{
#if USE_STD_HASH_MAP
    if (1) {
        measure_string_hashmap<StdHashMap<Key, Value>>
            ("stdext::hash_map", obj_size, iters);
    }
#endif

#if USE_STD_UNORDERED_MAP
    if (1) {
        measure_string_hashmap<std::unordered_map<Key, Value>>
            ("std::unordered_map", obj_size, iters);
    }
#endif

#if USE_SKA_BYTELL_HASH_MAP
    if (1) {
        measure_string_hashmap<ska::bytell_hash_map<Key, Value>>
            ("ska::bytell_hash_map", obj_size, iters);
    }
#endif

#if USE_SKA_FLAT_HASH_MAP
    if (1) {
        measure_string_hashmap<ska::flat_hash_map<Key, Value>>
            ("ska::flat_hash_map", obj_size, iters);
    }
#endif

#if USE_TSL_ROBIN_HOOD
    if (1) {
        measure_string_hashmap<tsl::robin_map<Key, Value>>
            ("tsl::robin_map", obj_size, iters);
    }
#endif

#if USE_JSTD_ROBIN_HASH_MAP
    if (1) {
        measure_string_hashmap<jstd::robin_hash_map<Key, Value>>
            ("jstd::robin_hash_map", obj_size, iters);
    }
#endif

#if USE_EMHASH5_HASH_MAP
    if (1) {
        measure_string_hashmap<emhash5::HashMap<Key, Value>>
            ("emhash5::HashMap", obj_size, iters);
    }
#endif

#if USE_EMHASH7_HASH_MAP
    if (1) {
        measure_string_hashmap<emhash7::HashMap<Key, Value>>
            ("emhash7::HashMap", obj_size, iters);
    }
#endif

#if USE_EMHASH8_HASH_MAP
    if (1) {
        measure_string_hashmap<emhash8::HashMap<Key, Value>>
            ("emhash8::HashMap", obj_size, iters);
    }
#endif

#if USE_ROBIN_HOOD_UNORDERED_MAP
    if (1) {
        measure_string_hashmap<robin_hood::unordered_map<Key, Value>>
            ("robin_hood::unordered_map", obj_size, iters);
    }
#endif

#if USE_ANKERL_UNORDERED_DENSE
    if (1) {
        measure_string_hashmap<ankerl::unordered_dense::map<Key, Value>>
            ("ankerl::unordered_dense::map", obj_size, iters);
    }
#endif

#if USE_ABSL_FLAT_HASH_MAP
    if (1) {
        measure_string_hashmap<absl::flat_hash_map<Key, Value>>
            ("absl::flat_hash_map", obj_size, iters);
    }
#endif

#if USE_ABSL_NODE_HASH_MAP
    if (1) {
        measure_string_hashmap<absl::node_hash_map<Key, Value>>
            ("absl::node_hash_map", obj_size, iters);
    }
#endif

#if USE_JSTD_GROUP16_FALT_MAP
    if (1) {
        measure_string_hashmap<jstd::group16_flat_map<Key, Value>>
            ("jstd::group16_flat_map", obj_size, iters);
    }
#endif

#if USE_JSTD_GROUP15_FALT_MAP
    if (1) {
        measure_string_hashmap<jstd::group15_flat_map<Key, Value>>
            ("jstd::group15_flat_map", obj_size, iters);
    }
#endif

#if USE_BOOST_UNORDERED_FLAT_MAP
    if (1) {
        measure_string_hashmap<boost::unordered::unordered_flat_map<Key, Value>>
            ("boost::unordered_flat_map", obj_size, iters);
    }
#endif
}

template <typename Key, typename Value>
void test_hashmap_by_name_for_string_view(const std::string & name, std::size_t obj_size, std::size_t iters)
{

}

template <typename Key, typename Value>
void test_all_hashmaps_for_string_view(std::size_t obj_size, std::size_t iters)
{

}

void benchmark_hashmap(const std::string & name, std::size_t iters)
{
#ifndef _DEBUG
    if (FLAGS_test_4_bytes && !FLAGS_test_string_only) {
        test_hashmap_by_name<std::uint32_t, std::uint32_t>(name, 4, iters / 1);
    }

    if (FLAGS_test_8_bytes && !FLAGS_test_string_only) {
        test_hashmap_by_name<std::uint64_t, std::uint64_t>(name, 8, iters / 2);
    }
#endif

    if (FLAGS_test_32_bytes) {
        test_hashmap_by_name_for_string<std::string, std::string>
            (name, sizeof(std::string), iters / 16);
    }

    if (FLAGS_test_16_bytes) {
        test_hashmap_by_name_for_string_view<jstd::string_view, jstd::string_view>
            (name, sizeof(jstd::string_view), iters / 8);
    }
}

void benchmark_all_hashmaps(std::size_t iters)
{
    // It would be nice to set these at run-time, but by setting them at
    // compile-time, we allow optimizations that make it as fast to use
    // a HashObject as it would be to use just a straight int/char
    // buffer.  To keep memory use similar, we normalize the number of
    // iterations based on size.
#ifndef _DEBUG
    if (FLAGS_test_4_bytes && !FLAGS_test_string_only) {
        test_all_hashmaps<std::uint32_t, std::uint32_t>(4, iters / 1);
    }

    if (FLAGS_test_8_bytes && !FLAGS_test_string_only) {
        test_all_hashmaps<std::uint64_t, std::uint64_t>(8, iters / 2);
    }
#endif

    if (FLAGS_test_32_bytes) {
        test_all_hashmaps_for_string<std::string, std::string>
            (sizeof(std::string), iters / 16);
    }

    if (FLAGS_test_16_bytes) {
        test_all_hashmaps_for_string_view<jstd::string_view, jstd::string_view>
            (sizeof(jstd::string_view), iters / 8);
    }
}

void std_hash_test()
{
    printf("#define HASH_MAP_FUNCTION = %s\n\n", PRINT_MACRO(HASH_MAP_FUNCTION));
    printf("sizeof(std::size_t) = %d\n\n", (int)sizeof(std::size_t));

#if (HASH_FUNCTION_ID == ID_STD_HASH)
    printf("std::hash<std::uint32_t>\n\n");
    for (std::uint32_t i = 0; i < 8; i++) {
        std::size_t hash_code = std::hash<std::uint32_t>()(i);
        printf("key = %3u, hash_code = 0x%08X%08X\n",
               i, UINT64_High(hash_code), UINT64_Low(hash_code));
    }
    printf("\n");

    printf("std::hash<std::uint64_t>\n\n");
    for (std::uint64_t i = 0; i < 8; i++) {
        std::size_t hash_code = std::hash<std::uint64_t>()(i);
        printf("key = %3" PRIu64 ", hash_code = 0x%08X%08X\n",
               i, UINT64_High(hash_code), UINT64_Low(hash_code));
    }
    printf("\n");

#if !defined(_MSC_VER)
    std::size_t diff = 0;
    for (std::size_t i = 0; i < 1024 * 1024; i++) {
        std::size_t hash_code = std::hash<std::size_t>()(i);
        if (hash_code != i)
            diff++;
    }
    printf("std::hash<std::size_t>[0, 1048576]: diff = %" PRIu64 "\n\n", diff);
#endif
#else
    printf("%s<std::uint32_t>\n\n", PRINT_MACRO(HASH_MAP_FUNCTION));
    for (std::uint32_t i = 0; i < 8; i++) {
        std::size_t hash_code = HASH_MAP_FUNCTION<std::uint32_t>()(i);
        printf("key = %3u, hash_code = 0x%08X%08X\n",
               i, UINT64_High(hash_code), UINT64_Low(hash_code));
    }
    printf("\n");

    printf("%s<std::uint64_t>\n\n", PRINT_MACRO(HASH_MAP_FUNCTION));
    for (std::uint64_t i = 0; i < 8; i++) {
        std::size_t hash_code = HASH_MAP_FUNCTION<std::uint64_t>()(i);
        printf("key = %3" PRIu64 ", hash_code = 0x%08X%08X\n",
               i, UINT64_High(hash_code), UINT64_Low(hash_code));
    }
    printf("\n");
#endif // HASH_FUNCTION_ID == ID_STD_HASH
}

template <typename Key, typename Value>
void print_need_store_hash(const std::string & key, const std::string & value)
{
    static constexpr bool hasMemberSwap = jstd::has_member_swap<Key, Key &>::value;
    static constexpr bool isPlainType = jstd::detail::is_plain_type<Key>::value;
    static constexpr bool needStoreHash = jstd::robin_hash_map<Key, Value>::kDetectStoreHash;

    printf("jstd::has_member_swap<%s>::value = %s\n",
           key.c_str(), (hasMemberSwap ? "True" : "False"));
    printf("jstd::detail::is_plain_type<%s>::value = %s\n",
           key.c_str(), (isPlainType ? "True" : "False"));
    printf("jstd::robin_hash_map<%s, %s>::kDetectStoreHash = %s\n",
           key.c_str(), value.c_str(), (needStoreHash ? "True" : "False"));
    printf("\n");
}

void need_store_hash_test()
{
    print_need_store_hash<std::uint32_t, std::uint32_t>("std::uint32_t", "std::uint32_t");
    print_need_store_hash<std::uint64_t, std::uint64_t>("std::uint64_t", "std::uint64_t");
    print_need_store_hash<std::string, std::string>("std::string", "std::string");
    print_need_store_hash<jstd::string_view, jstd::string_view>("jstd::string_view", "jstd::string_view");
}

template <typename Key, typename Value>
bool is_layout_compatible_pair()
{
    static constexpr bool isCompatibleLayout =
        jstd::is_layout_compatible_pair<std::pair<const Key, Value>, std::pair<Key, Value>>::value;
    return isCompatibleLayout;
}

template <typename Key, typename Value>
void is_compatible_layout_ex_test(const std::string & key, const std::string & value)
{
    static constexpr bool isStandardLayoutKey = std::is_standard_layout<Key>::value;
    static constexpr bool isStandardLayoutPair = std::is_standard_layout<std::pair<Key, Value>>::value;
    static constexpr bool isStandardLayoutConstPair = std::is_standard_layout<std::pair<const Key, Value>>::value;

    static constexpr bool isLayoutCompatiblePair =
        jstd::is_layout_compatible_kv<Key, Value>::template isLayoutCompatible<std::pair<Key, Value>>();
    static constexpr bool isLayoutCompatibleConstPair =
        jstd::is_layout_compatible_kv<Key, Value>::template isLayoutCompatible<std::pair<const Key, Value>>();

    static constexpr bool isLayoutCompatibleKV = jstd::is_layout_compatible_kv<Key, Value>::value;

    printf("jstd::is_standard_layout<%s>::value = %s\n",
           key.c_str(), (isStandardLayoutKey ? "True" : "False"));
    printf("jstd::is_standard_layout<std::pair<%s, %s>>::value = %s\n",
           key.c_str(), value.c_str(), (isStandardLayoutPair ? "True" : "False"));
    printf("jstd::is_standard_layout<std::pair<const %s, %s>>::value = %s\n",
           key.c_str(), value.c_str(), (isStandardLayoutConstPair ? "True" : "False"));

    printf("jstd::is_layout_compatible_kv::isLayoutCompatible<std::pair<%s, %s>>() = %s\n",
           key.c_str(), value.c_str(), (isLayoutCompatiblePair ? "True" : "False"));
    printf("jstd::is_layout_compatible_kv::isLayoutCompatible<std::pair<const %s, %s>>() = %s\n",
           key.c_str(), value.c_str(), (isLayoutCompatibleConstPair ? "True" : "False"));

    printf("jstd::is_layout_compatible_kv<%s, %s>::value = %s\n",
           key.c_str(), value.c_str(), (isLayoutCompatibleKV ? "True" : "False"));
    printf("\n");
}

void is_compatible_layout_test()
{
    bool isLayoutCompatibleKV, isLayoutCompatiblePair;

    isLayoutCompatibleKV = jstd::is_layout_compatible_kv<int, int>::value;
    isLayoutCompatiblePair = is_layout_compatible_pair<int, int>();
    printf("jstd::is_layout_compatible_kv<int, int> = %s\n",
            (isLayoutCompatibleKV ? "True" : "False"));
    printf("jstd::is_layout_compatible_pair<int, int> = %s\n",
            (isLayoutCompatiblePair ? "True" : "False"));
    printf("\n");

    isLayoutCompatibleKV = jstd::is_layout_compatible_kv<size_t, size_t>::value;
    isLayoutCompatiblePair = is_layout_compatible_pair<size_t, size_t>();
    printf("jstd::is_layout_compatible_kv<size_t, size_t> = %s\n",
            (isLayoutCompatibleKV ? "True" : "False"));
    printf("jstd::is_layout_compatible_pair<size_t, size_t> = %s\n",
            (isLayoutCompatiblePair ? "True" : "False"));
    printf("\n");

    isLayoutCompatibleKV = jstd::is_layout_compatible_kv<std::string, std::string>::value;
    isLayoutCompatiblePair = is_layout_compatible_pair<std::string, std::string>();
    printf("jstd::is_layout_compatible_kv<std::string, std::string> = %s\n",
            (isLayoutCompatibleKV ? "True" : "False"));
    printf("jstd::is_layout_compatible_pair<std::string, std::string> = %s\n",
            (isLayoutCompatiblePair ? "True" : "False"));
    printf("\n");
    if (!isLayoutCompatibleKV || !isLayoutCompatiblePair) {
        is_compatible_layout_ex_test<std::string, std::string>("std::string", "std::string");
    }

    isLayoutCompatibleKV = jstd::is_layout_compatible_kv<jstd::string_view, jstd::string_view>::value;
    isLayoutCompatiblePair = is_layout_compatible_pair<jstd::string_view, jstd::string_view>();
    printf("jstd::is_layout_compatible_kv<jstd::string_view, jstd::string_view> = %s\n",
            (isLayoutCompatibleKV ? "True" : "False"));
    printf("jstd::is_layout_compatible_pair<jstd::string_view, jstd::string_view> = %s\n",
            (isLayoutCompatiblePair ? "True" : "False"));
    printf("\n");
}

void strtolower(char * s) {
    for (int i = 0; s[i]; i++)
        s[i] = tolower(s[i]);
}

bool strisdigits(char * str)
{
    while (*str != '\0') {
        char ch = *str;
        if (ch >= '0' && ch <= '9')
            str++;
        else
            return false;
    }
    return true;
}

int main(int argc, char * argv[])
{
    jstd::RandomGen   RandomGen(20200831);
    jstd::MtRandomGen mtRandomGen(20200831);

    std::size_t iters = kDefaultIters;
    std::string name;
    if (argc > 1) {
        char arg1[256] = { 0 };
        char arg2[256] = { 0 };
        char arg3[256] = { 0 };
        strcpy(arg1, argv[1]);
        strtolower(arg1);
        if (argc > 2) {
            strcpy(arg2, argv[2]);
            strtolower(arg2);
        }
        if (argc > 3) {
            strcpy(arg3, argv[3]);
            strtolower(arg3);
        }

        if (0) {
            // Dummy header
        } else if (argc > 3) {
            if (strcmp(arg1, "string") == 0) {
                FLAGS_test_string_only = true;
                if (!strisdigits(arg2)) {
                    name = arg2;
                    if (strisdigits(arg3))
                        iters = ::atoi(arg3);
                } else {
                    // first arg is # of iterations
                    iters = ::atoi(arg2);
                }
            } else if (!strisdigits(arg1)) {
                name = arg1;
                if (strisdigits(arg2))
                    iters = ::atoi(arg2);
            } else {
                // first arg is # of iterations
                iters = ::atoi(arg1);
            }
        } else if (argc > 2) {
            if (strcmp(arg1, "string") == 0) {
                FLAGS_test_string_only = true;
                if (!strisdigits(arg2)) {
                    name = arg2;
                } else {
                    // first arg is # of iterations
                    iters = ::atoi(arg2);
                }
            } else if (!strisdigits(arg1)) {
                name = arg1;
                if (strisdigits(arg2))
                    iters = ::atoi(arg2);
            } else {
                // first arg is # of iterations
                iters = ::atoi(arg1);
            }
        } else if (argc > 1) {
            if (strcmp(arg1, "string") == 0) {
                FLAGS_test_string_only = true;
            } else if (!strisdigits(arg1)) {
                name = arg1;
            } else {
                // first arg is # of iterations
                iters = ::atoi(arg1);
            }
        }

        if (iters == 0) {
            printf("Error: iters parameter is wrong.");
            exit(1);
        }
    }

    jtest::CPU::warm_up(1000);

    if (1) { std_hash_test(); }
    if (0) { need_store_hash_test(); }
    if (0) { is_compatible_layout_test(); }

    if (name == "") {
        printf("---------------------- benchmark_all_hashmaps (iters = %u) ----------------------\n\n",
               (std::uint32_t)iters);
        benchmark_all_hashmaps(iters);
    } else {
        printf("---------------------- benchmark_a_hashmap (iters = %u) ----------------------\n\n",
               (std::uint32_t)iters);
        benchmark_hashmap(name, iters);
    }

    printf("-----------------------------------------------------------------------------\n\n");

#if defined(_MSC_VER) && defined(_DEBUG)
    jstd::Console::ReadKey();
#endif
    return 0;
}
