
/************************************************************************************

  CC BY-SA 4.0 License

  Copyright (c) 2020-2022 XiongHui Guo (gz_shines at msn.com)

  https://github.com/shines77/hashmap-benchmark
  https://gitee.com/shines77/hashmap-benchmark

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
#include <string>
#include <cstring>
#include <atomic>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

#define USE_JSTD_HASH_TABLE         0
#define USE_JSTD_DICTIONARY         0

#define USE_STD_HASH_MAP            0
#define USE_STD_UNORDERED_MAP       1
#define USE_JSTD_FLAT16_HASH_MAP    1
#define USE_JSTD_ROBIN16_HASH_MAP   1
#define USE_SKA_FLAT_HASH_MAP       1
#define USE_SKA_BYTELL_HASH_MAP     0
#define USE_ABSL_FLAT_HASH_MAP      1
#define USE_ABSL_NODE_HASH_MAP      0

#ifdef _MSC_VER
#undef USE_ABSL_FLAT_HASH_MAP
#undef USE_ABSL_NODE_HASH_MAP
#endif

/* SIMD support features */
#define JSTD_HAVE_MMX           1
#define JSTD_HAVE_SSE           1
#define JSTD_HAVE_SSE2          1
#define JSTD_HAVE_SSE3          1
#define JSTD_HAVE_SSSE3         1
#define JSTD_HAVE_SSE4          1
#define JSTD_HAVE_SSE4A         1
#define JSTD_HAVE_SSE4_1        1
#define JSTD_HAVE_SSE4_2        1

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

#if USE_STD_UNORDERED_MAP
#include <unordered_map>
#endif

#if USE_JSTD_FLAT16_HASH_MAP
#include <jstd/hashmap/flat16_hash_map.h>
#endif
#if USE_JSTD_ROBIN16_HASH_MAP
#include <jstd/hashmap/robin16_hash_map.h>
#endif
#if USE_SKA_FLAT_HASH_MAP
#include <flat_hash_map/flat_hash_map.hpp>
#endif
#if USE_SKA_BYTELL_HASH_MAP
#include <flat_hash_map/bytell_hash_map.hpp>
#endif
#if USE_ABSL_FLAT_HASH_MAP
#include <absl/container/flat_hash_map.h>
#endif
#if USE_ABSL_NODE_HASH_MAP
#include <absl/container/node_hash_map.h>
#endif
#include <jstd/hashmap/hashmap_analyzer.h>
#include <jstd/hasher/hashes.h>
#include <jstd/hasher/hash_helper.h>
#include <jstd/string/string_view.h>
#include <jstd/string/string_view_array.h>
#include <jstd/system/Console.h>
#include <jstd/system/RandomGen.h>
#include <jstd/test/StopWatch.h>
#include <jstd/test/CPUWarmUp.h>
#include <jstd/test/ProcessMemInfo.h>
#include <jstd/test/ReadRss.h>

#define USE_STAT_COUNTER        1

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
#define HASH_FUNCTION_ID        ID_STD_HASH
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
#define UINT64_High(u64)        ((uint32_t)(u64 >> 32))
#endif

#ifndef UINT64_Low
#define UINT64_Low(u64)         ((uint32_t)(u64 & 0x00000000FFFFFFFFull))
#endif

#pragma message(PRINT_MACRO_VAR(HASH_MAP_FUNCTION))

#if defined(_MSC_VER)
static bool FLAGS_test_std_hash_map = false;
#else
static bool FLAGS_test_std_hash_map = false;
#endif
static bool FLAGS_test_std_unordered_map = true;
static bool FLAGS_test_jstd_flat16_hash_map = true;
static bool FLAGS_test_jstd_robin16_hash_map = true;
static bool FLAGS_test_ska_flat_hash_map = true;
static bool FLAGS_test_ska_bytell_hash_map = true;
static bool FLAGS_test_absl_flat_hash_map = true;
static bool FLAGS_test_absl_node_hash_map = true;
static bool FLAGS_test_map = false;

static constexpr bool FLAGS_test_4_bytes = true;
static constexpr bool FLAGS_test_8_bytes = true;
static constexpr bool FLAGS_test_16_bytes = true;
static constexpr bool FLAGS_test_256_bytes = true;

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
#if USE_CTOR_COUNTER
    g_num_constructor = 0;
#endif
#endif
}

static inline
std::size_t CurrentMemoryUsage()
{
    return jtest::getCurrentRSS();
}

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
    result_type operator () (const Argument & value) const {
        std::hash<Argument> hasher;
        return static_cast<result_type>(hasher(value));
    }
};

template <typename T>
struct IntegalHash
{
    typedef T           argument_type;
    typedef std::size_t result_type;

    template <typename UInt32, typename std::enable_if<
                                (std::is_integral<UInt32>::value &&
                                (sizeof(UInt32) <= 4))>::type * = nullptr>
    result_type operator () (UInt32 value) const noexcept {
        result_type hash = (result_type)(((std::uint64_t)value * 2654435769ul) >> 12);
        return hash;
    }

    template <typename UInt64, typename std::enable_if<
                                (std::is_integral<UInt64>::value &&
                                (sizeof(UInt64) > 4 && sizeof(UInt64) <= 8))>::type * = nullptr>
    result_type operator () (UInt64 value) const noexcept {
        result_type hash = (result_type)(((std::uint64_t)value * 11400714819323198485ull) >> 28);
        return hash;
    }

    template <typename Argument, typename std::enable_if<
                                  (!std::is_integral<Argument>::value ||
                                  sizeof(Argument) > 8)>::type * = nullptr>
    result_type operator () (const Argument & value) const noexcept {
        std::hash<Argument> hasher;
        return static_cast<result_type>(hasher(value));
    }
};

template <typename T>
struct MumHash
{
    typedef T           argument_type;
    typedef std::size_t result_type;

    template <typename Integer, typename std::enable_if<
                                (std::is_integral<Integer>::value &&
                                (sizeof(Integer) <= 8))>::type * = nullptr>
    result_type operator () (Integer value) const noexcept {
        result_type hash = (result_type)(jstd::hashes::mum_hash64((std::uint64_t)value, 11400714819323198485ull));
        return hash;
    }

    template <typename Argument, typename std::enable_if<
                                  (!std::is_integral<Argument>::value ||
                                  sizeof(Argument) > 8)>::type * = nullptr>
    result_type operator () (const Argument & value) const noexcept {
        std::hash<Argument> hasher;
        return static_cast<result_type>(hasher(value));
    }
};

} // namespace test

/*
 * These are the objects we hash.  Size is the size of the object
 * (must be > sizeof(int).  Hashsize is how many of these bytes we
 * use when hashing (must be > sizeof(int) and < Size).
 */
template <typename Key, std::size_t Size, std::size_t HashSize>
class HashObject {
public:
    typedef Key                             key_type;
    typedef HashObject<Key, Size, HashSize> this_type;

    static constexpr std::size_t cSize = Size;
    static constexpr std::size_t cHashSize = HashSize;

    static constexpr std::size_t kMinSize = jstd::cmax(sizeof(key_type), sizeof(int));
    static constexpr std::size_t kSize = jstd::cmax(Size, kMinSize);
    static constexpr std::size_t kHashSize = jstd::cmin(jstd::cmax(HashSize, kMinSize), kSize);
    static constexpr std::size_t kBufLen = (kSize > sizeof(key_type)) ? (kSize - sizeof(key_type)) : 0;
    static constexpr std::size_t kHashLen = (kHashSize > sizeof(key_type)) ? (kHashSize - sizeof(key_type)) : 0;

private:
    key_type key_;   // the key used for hashing
    char buffer_[kBufLen];

public:
    HashObject() noexcept : key_(0) {
        std::memset(this->buffer_, 0, sizeof(char) * kBufLen);
#if (USE_STAT_COUNTER != 0) && (USE_CTOR_COUNTER != 0)
        g_num_constructor++;
#endif
    }
    HashObject(key_type key) noexcept : key_(key) {
        std::memset(this->buffer_, (int)(key & 0xFFUL), sizeof(char) * kBufLen);   // a "random" char
#if (USE_STAT_COUNTER != 0) && (USE_CTOR_COUNTER != 0)
        g_num_constructor++;
#endif
    }
    HashObject(const this_type & that) noexcept {
        operator = (that);
    }

    ~HashObject() = default;

    this_type & operator = (const this_type & that) noexcept {
#if USE_STAT_COUNTER
        g_num_copies++;
#endif
        this->key_ = that.key_;
        std::memcpy(this->buffer_, that.buffer_, sizeof(char) * kBufLen);
        return *this;
    }

    key_type key() const noexcept {
        return this->key_;
    }

    const char * buffer() const noexcept {
        return (const char *)&this->buffer_[0];
    }

    std::size_t Hash() const {
#if USE_STAT_COUNTER
        g_num_hashes++;
#endif
        std::size_t hash_val = static_cast<std::size_t>(this->key_);
#if 1
        for (std::size_t i = 0; i < kHashLen; ++i) {
            hash_val += this->buffer_[i];
        }
#else
        hash_val += static_cast<std::size_t>((this->key_ & 0xFFUL) * kHashLen);
#endif
        return static_cast<std::size_t>(
            HASH_MAP_FUNCTION<std::size_t>()(hash_val)
        );
    }

    bool operator == (const this_type & that) const noexcept {
        return this->key_ == that.key_;
    }
    bool operator < (const this_type & that) const noexcept {
        return this->key_ < that.key_;
    }
    bool operator <= (const this_type & that) const noexcept {
        return this->key_ <= that.key_;
    }

    friend std::ostream & operator << (std::ostream & out, const this_type & obj) {
        out << "HashObject(" << obj.key() << ", \"" << obj.buffer() << "\")";
        return out;
    }
};

// A specialization for the case sizeof(buffer_) == 0
template <>
class HashObject<std::uint32_t, sizeof(std::uint32_t), sizeof(std::uint32_t)> {
public:
    typedef std::uint32_t   key_type;
    typedef HashObject<std::uint32_t, sizeof(std::uint32_t), sizeof(std::uint32_t)>
                            this_type;

    static constexpr std::size_t cSize = sizeof(std::uint32_t);
    static constexpr std::size_t cHashSize = sizeof(std::uint32_t);

private:
    std::uint32_t key_;   // the key used for hashing

public:
    HashObject() noexcept : key_(0) {
#if (USE_STAT_COUNTER != 0) && (USE_CTOR_COUNTER != 0)
        g_num_constructor++;
#endif
    }
    HashObject(std::uint32_t key) noexcept : key_(key) {
#if (USE_STAT_COUNTER != 0) && (USE_CTOR_COUNTER != 0)
        g_num_constructor++;
#endif
    }
    HashObject(const this_type & that) noexcept {
        operator = (that);
    }

    ~HashObject() = default;

    this_type & operator = (const this_type & that) noexcept {
#if USE_STAT_COUNTER
        g_num_copies++;
#endif
        this->key_ = that.key_;
        return *this;
    }

    key_type key() const noexcept {
        return this->key_;
    }

    const char * buffer() const noexcept {
        return nullptr;
    }

    std::size_t Hash() const {
#if USE_STAT_COUNTER
        g_num_hashes++;
#endif
        return static_cast<std::size_t>(
            HASH_MAP_FUNCTION<std::uint32_t>()(this->key_)
        );
    }

    bool operator == (const this_type & that) const noexcept {
        return this->key_ == that.key_;
    }
    bool operator < (const this_type & that) const noexcept {
        return this->key_ < that.key_;
    }
    bool operator <= (const this_type & that) const noexcept {
        return this->key_ <= that.key_;
    }

    friend std::ostream & operator << (std::ostream & out, const this_type & obj) {
        out << "HashObject(" << obj.key() << ")";
        return out;
    }
};

#if defined(WIN64) || defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) \
 || defined(__amd64__) || defined(__x86_64__) || defined(__LP64__)

// A specialization for the case sizeof(buffer_) == 0
template <>
class HashObject<std::size_t, sizeof(std::size_t), sizeof(std::size_t)> {
public:
    typedef std::size_t     key_type;
    typedef HashObject<std::size_t, sizeof(std::size_t), sizeof(std::size_t)>
                            this_type;

    static constexpr std::size_t cSize = sizeof(std::size_t);
    static constexpr std::size_t cHashSize = sizeof(std::size_t);

private:
    std::size_t key_;   // the key used for hashing

public:
    HashObject() noexcept : key_(0) {
#if (USE_STAT_COUNTER != 0) && (USE_CTOR_COUNTER != 0)
        g_num_constructor++;
#endif
    }
    HashObject(std::size_t key) noexcept : key_(key) {
#if (USE_STAT_COUNTER != 0) && (USE_CTOR_COUNTER != 0)
        g_num_constructor++;
#endif
    }
    HashObject(const this_type & that) noexcept {
        operator = (that);
    }

    ~HashObject() = default;

    this_type & operator = (const this_type & that) noexcept {
#if USE_STAT_COUNTER
        g_num_copies++;
#endif
        this->key_ = that.key_;
        return *this;
    }

    key_type key() const noexcept {
        return this->key_;
    }

    const char * buffer() const noexcept {
        return nullptr;
    }

    std::size_t Hash() const {
#if USE_STAT_COUNTER
        g_num_hashes++;
#endif
        return static_cast<std::size_t>(
            HASH_MAP_FUNCTION<std::size_t>()(this->key_)
        );
    }

    bool operator == (const this_type & that) const noexcept {
        return this->key_ == that.key_;
    }
    bool operator < (const this_type & that) const noexcept {
        return this->key_ < that.key_;
    }
    bool operator <= (const this_type & that) const noexcept {
        return this->key_ <= that.key_;
    }

    friend std::ostream & operator << (std::ostream & out, const this_type & obj) {
        out << "HashObject(" << obj.key() << ")";
        return out;
    }
};

#endif // _WIN64 || __amd64__

template <typename Key, std::size_t Size = sizeof(Key),
                        std::size_t HashSize = sizeof(Key),
                        bool is_special = false>
class HashFn {
public:
    typedef Key                             key_type;
    typedef HashObject<Key, Size, HashSize> argument_type;
    typedef std::size_t                     result_type;

    typedef std::pair<argument_type, Key>   pair_type;
    typedef std::pair<argument_type *, Key> pair_type_2nd;

    // These two public members are required by msvc.  4 and 8 are defaults.
    static const std::size_t bucket_size = 4;
    static const std::size_t min_buckets = 8;

    result_type operator () (const key_type & key) {
        if (is_special)
            return static_cast<result_type>(test::MumHash<key_type>()(key));
        else
            return static_cast<result_type>(HASH_MAP_FUNCTION<key_type>()(key));
    }

    result_type operator () (const key_type & key) const {
        if (is_special)
            return static_cast<result_type>(test::MumHash<key_type>()(key));
        else
            return static_cast<result_type>(HASH_MAP_FUNCTION<key_type>()(key));
    }

    result_type operator () (const pair_type & pair) {
        return static_cast<result_type>(pair.first.Hash());
    }

    result_type operator () (const pair_type & pair) const {
        return static_cast<result_type>(pair.first.Hash());
    }

    result_type operator () (const pair_type_2nd & pair) {
        return static_cast<result_type>(pair.first->Hash());
    }

    result_type operator () (const pair_type_2nd & pair) const {
        return static_cast<result_type>(pair.first->Hash());
    }

    result_type operator () (const argument_type & obj) {
        if (is_special)
            return static_cast<result_type>(test::MumHash<key_type>()(obj.Hash()));
        else
            return static_cast<result_type>(obj.Hash());
    }

    result_type operator () (const argument_type & obj) const {
        if (is_special)
            return static_cast<result_type>(test::MumHash<key_type>()(obj.Hash()));
        else
            return static_cast<result_type>(obj.Hash());
    }

#if 1
    result_type operator () (const argument_type * obj) {
        return reinterpret_cast<result_type>(obj);
    }

    // Do the identity hash for pointers.
    result_type operator () (const argument_type * obj) const {
        return reinterpret_cast<result_type>(obj);
    }
#endif

    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const std::pair<HashObject<UKey, nSize, nHashSize>, key_type> & pair) {
        return static_cast<result_type>(pair.first.Hash());
    }

    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const std::pair<HashObject<UKey, nSize, nHashSize>, key_type> & pair) const {
        return static_cast<result_type>(pair.first.Hash());
    }

    // Less operator for MSVC's hash containers.
    bool operator () (const argument_type & a, argument_type & b) const {
        return (a < b);
    }

    bool operator () (const argument_type * a, const argument_type * b) const {
        return (a < b);
    }

    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const HashObject<UKey, nSize, nHashSize> & obj) {
        if (is_special)
            return static_cast<result_type>(test::MumHash<key_type>()(obj.Hash()));
        else
            return static_cast<result_type>(obj.Hash());
    }

    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const HashObject<UKey, nSize, nHashSize> & obj) const {
        if (is_special)
            return static_cast<result_type>(test::MumHash<key_type>()(obj.Hash()));
        else
            return static_cast<result_type>(obj.Hash());
    }

#if 1
    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const HashObject<UKey, nSize, nHashSize> * obj) {
        return reinterpret_cast<result_type>(obj);
    }

    // Do the identity hash for pointers.
    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const HashObject<UKey, nSize, nHashSize> * obj) const {
        return reinterpret_cast<result_type>(obj);
    }
#endif

    // Less operator for MSVC's hash containers.
    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    bool operator () (const HashObject<UKey, nSize, nHashSize> & a,
                      const HashObject<UKey, nSize, nHashSize> & b) const {
        return (a < b);
    }

    template <typename UKey, std::size_t nSize, std::size_t nHashSize>
    bool operator () (const HashObject<UKey, nSize, nHashSize> * a,
                      const HashObject<UKey, nSize, nHashSize> * b) const {
        return (a < b);
    }
};

#if 0

namespace std {

template <typename Key, std::size_t Size, std::size_t HashSize>
struct hash<HashObject<Key, Size, HashSize>> {
    typedef Key                             key_type;
    typedef HashObject<Key, Size, HashSize> argument_type;
    typedef std::size_t                     result_type;

    // These two public members are required by msvc.  4 and 8 are defaults.
    static const std::size_t bucket_size = 4;
    static const std::size_t min_buckets = 8;

    result_type operator () (const argument_type & obj) const {
        return static_cast<result_type>(obj.Hash());
    }

    // Do the identity hash for pointers.
    result_type operator () (const argument_type * obj) const {
        return reinterpret_cast<result_type>(obj);
    }

    // Less operator for MSVC's hash containers.
    bool operator () (const argument_type & a, argument_type & b) const {
        return (a < b);
    }

    bool operator () (const argument_type * a, const argument_type * b) const {
        return (a < b);
    }

    template <std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const HashObject<key_type, nSize, nHashSize> & obj) const {
        return static_cast<result_type>(obj.Hash());
    }

    // Do the identity hash for pointers.
    template <std::size_t nSize, std::size_t nHashSize>
    result_type operator () (const HashObject<key_type, nSize, nHashSize> * obj) const {
        return reinterpret_cast<result_type>(obj);
    }

    // Less operator for MSVC's hash containers.
    template <std::size_t nSize, std::size_t nHashSize>
    bool operator () (const HashObject<key_type, nSize, nHashSize> & a,
                      const HashObject<key_type, nSize, nHashSize> & b) const {
        return (a < b);
    }

    template <std::size_t nSize, std::size_t nHashSize>
    bool operator () (const HashObject<key_type, nSize, nHashSize> * a,
                      const HashObject<key_type, nSize, nHashSize> * b) const {
        return (a < b);
    }
};

} // namespace std

#endif

#if 0

namespace std {

// Let the hashtable implementations know it can use an optimized memcpy,
// because the compiler defines both the destructor and copy constructor.

// is_trivially_copyable

template <std::size_t Size, std::size_t HashSize>
struct is_trivially_copyable< HashObject<std::uint32_t, Size, HashSize> > : true_type { };

template <std::size_t Size, std::size_t HashSize>
struct is_trivially_copyable< HashObject<std::uint64_t, Size, HashSize> > : true_type { };

template <>
struct is_trivially_copyable< HashObject<std::uint32_t, 4, 4> > : true_type { };

template <>
struct is_trivially_copyable< HashObject<std::uint64_t, 8, 8> > : true_type { };

template <>
struct is_trivially_copyable< HashObject<std::size_t, 16, 16> > : true_type { };

template <>
struct is_trivially_copyable< HashObject<std::size_t, 256, 32> > : true_type { };

// is_trivially_destructible

template <std::size_t Size, std::size_t HashSize>
struct is_trivially_destructible< HashObject<std::uint32_t, Size, HashSize> > : true_type { };

template <std::size_t Size, std::size_t HashSize>
struct is_trivially_destructible< HashObject<std::uint64_t, Size, HashSize> > : true_type { };

template <>
struct is_trivially_destructible< HashObject<std::uint32_t, 4, 4> > : true_type { };

template <>
struct is_trivially_destructible< HashObject<std::uint64_t, 8, 8> > : true_type { };

template <>
struct is_trivially_destructible< HashObject<std::size_t, 16, 16> > : true_type { };

template <>
struct is_trivially_destructible< HashObject<std::size_t, 256, 32> > : true_type { };

} // namespace std

#endif

#if 0

namespace google {

// Let the hashtable implementations know it can use an optimized memcpy,
// because the compiler defines both the destructor and copy constructor.

// has_trivial_copy

template <std::size_t Size, std::size_t HashSize>
struct has_trivial_copy< HashObject<std::uint32_t, Size, HashSize> > : true_type { };

template <std::size_t Size, std::size_t HashSize>
struct has_trivial_copy< HashObject<std::uint64_t, Size, HashSize> > : true_type { };

// has_trivial_destructor

template <std::size_t Size, std::size_t HashSize>
struct has_trivial_destructor< HashObject<std::uint32_t, Size, HashSize> > : true_type { };

template <std::size_t Size, std::size_t HashSize>
struct has_trivial_destructor< HashObject<std::uint64_t, Size, HashSize> > : true_type { };

} // namespace google

#endif

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
    if (jstd::has_static_name<Container>::value)
        printf(" %-36s  ", jstd::call_static_name<Container>::name().c_str());
    else
        printf(" %-36s  ", "std::unordered_map<K, V>");
    printf("sum = %-10" PRIuPTR "  time: %8.3f ms\n", checksum, elapsedTime);
}

static void report_result(char const * title, double ut, double lf, std::size_t iters,
                          size_t start_memory, size_t end_memory) {
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
    printf("%-35s %8.2f ns  lf=%0.3f  %s\n", title, (ut * 1000000000.0 / iters), lf, heap);
#else
  #if USE_CTOR_COUNTER
    printf("%-35s %8.2f ns  (%8" PRIuPTR " hashes, %8" PRIuPTR " copies, %8" PRIuPTR " ctor) lf=%0.3f %s\n",
           title, (ut * 1000000000.0 / iters),
           g_num_hashes, g_num_copies, g_num_constructor,
           lf, heap);
  #else
    printf("%-35s %8.2f ns  (%8" PRIuPTR " hashes, %8" PRIuPTR " copies) lf=%0.3f %s\n",
           title, (ut * 1000000000.0 / iters),
           g_num_hashes, g_num_copies,
           lf, heap);
  #endif
#endif
    ::fflush(stdout);
}

#if 0

// Apply a pseudorandom permutation to the given vector.
template <typename Vector>
void shuffle_vector(Vector & vector, int seed = 0) {
    // shuffle
    //::srand(9);
    if (seed == 0)
        seed = 20200831;
    jstd::RandomGen RandomGen(seed);
    for (std::size_t n = vector.size(); n >= 2; n--) {
        std::size_t rnd_idx = std::size_t(jstd::RandomGen::nextUInt32()) % n;
        std::swap(vector[n - 1], vector[rnd_idx]);
    }
}

#else

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

#endif

/* The implementation of test routine */
#include "time_hash_map_func.hpp"

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

    //------------------------------------------------------------

    typedef typename MapType::mapped_type mapped_type;
    std::vector<mapped_type> rndIndices;
    rndIndices.reserve(iters);
    for (mapped_type i = 0; i < iters; i++) {
        rndIndices.push_back(i);
    }
    // Seed = 20220714
    shuffle_vector(rndIndices, 20220714);

    //------------------------------------------------------------

    if (1) map_sequential_find_success<MapType>(iters);
    if (1) map_sequential_find_random<MapType>(iters);
    if (1) map_sequential_find_failed<MapType>(iters);
    if (1) map_sequential_find_empty<MapType>(iters);
    if (1) printf("\n");

    if (1) map_random_find_sequential<MapType>(iters, rndIndices);
    if (1) map_random_find_random<MapType>(iters, rndIndices);
    if (1) map_random_find_failed<MapType>(iters, rndIndices);
    if (1) map_random_find_empty<MapType>(iters, rndIndices);
    if (1) printf("\n");

    //------------------------------------------------------------

    if (1) map_sequential_insert<MapType>(iters);
    if (1) map_sequential_insert_predicted<MapType>(iters);
    if (1) map_sequential_insert_replace<MapType>(iters);
    if (1) printf("\n");

    if (1) map_sequential_emplace<MapType>(iters);
    if (1) map_sequential_emplace_predicted<MapType>(iters);
    if (1) map_sequential_emplace_replace<MapType>(iters);
    if (1) printf("\n");

    if (1) map_sequential_operator<MapType>(iters);
    if (1) map_sequential_operator_predicted<MapType>(iters);
    if (1) map_sequential_operator_replace<MapType>(iters);
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

    if (1) map_sequential_erase<MapType>(iters);
    if (1) map_sequential_erase_failed<MapType>(iters);
    if (1) map_sequential_toggle<MapType>(iters);
    if (1) map_sequential_iterate<MapType>(iters);
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

template <typename HashObj, typename Value>
static void test_all_hashmaps(std::size_t obj_size, std::size_t iters) {
    const bool has_stress_hash_function = (obj_size <= 8);

#if USE_STD_HASH_MAP
    if (FLAGS_test_std_hash_map) {
        measure_hashmap<StdHashMap<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        StdHashMap<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "stdext::hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_STD_UNORDERED_MAP
    if (FLAGS_test_std_unordered_map) {
        measure_hashmap<std::unordered_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        std::unordered_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "std::unordered_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_FLAT16_HASH_MAP
    if (FLAGS_test_jstd_flat16_hash_map) {
        measure_hashmap<jstd::flat16_hash_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        jstd::flat16_hash_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "jstd::flat16_hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_JSTD_ROBIN16_HASH_MAP
    if (FLAGS_test_jstd_robin16_hash_map) {
        measure_hashmap<jstd::robin16_hash_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        jstd::robin16_hash_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "jstd::robin16_hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_SKA_FLAT_HASH_MAP
    if (FLAGS_test_ska_flat_hash_map) {
        measure_hashmap<ska::flat_hash_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        ska::flat_hash_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "ska::flat_hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_SKA_BYTELL_HASH_MAP
    if (FLAGS_test_ska_bytell_hash_map) {
        measure_hashmap<ska::bytell_hash_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        ska::bytell_hash_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "ska::bytell_hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_ABSL_FLAT_HASH_MAP
    if (FLAGS_test_absl_flat_hash_map) {
        measure_hashmap<absl::flat_hash_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize, true>>,
                        absl::flat_hash_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize, true>>
                        >(
            "absl::flat_hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif

#if USE_ABSL_NODE_HASH_MAP
    if (FLAGS_test_absl_node_hash_map) {
        measure_hashmap<absl::node_hash_map<HashObj,   Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>,
                        absl::node_hash_map<HashObj *, Value, HashFn<Value, HashObj::cSize, HashObj::cHashSize>>
                        >(
            "absl::node_hash_map<K, V>", obj_size, 0, iters, has_stress_hash_function);
    }
#endif
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

void std_hash_test()
{
    printf("#define HASH_MAP_FUNCTION = %s\n\n", PRINT_MACRO(HASH_MAP_FUNCTION));

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

#if (HASH_FUNCTION_ID != ID_STD_HASH)
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
#endif
}

int main(int argc, char * argv[])
{
    jstd::RandomGen   RandomGen(20200831);
    jstd::MtRandomGen mtRandomGen(20200831);

    std::size_t iters = kDefaultIters;
    if (argc > 1) {
        // first arg is # of iterations
        iters = ::atoi(argv[1]);
    }

    jtest::CPU::warm_up(1000);

    if (1) { std_hash_test(); }

    if (1)
    {
        printf("-------------------------- benchmark_all_hashmaps(iters) ---------------------------\n\n");
        benchmark_all_hashmaps(iters);
    }

    printf("------------------------------------------------------------------------------------\n\n");

#if defined(_MSC_VER) && defined(_DEBUG)
    jstd::Console::ReadKey();
#endif
    return 0;
}
