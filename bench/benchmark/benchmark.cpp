
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
#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#define USE_JSTD_HASH_TABLE         0
#define USE_JSTD_DICTIONARY         0

#define USE_JSTD_FLAT16_HASH_MAP    1
#define USE_JSTD_ROBIN16_HASH_MAP   0
#define USE_JSTD_ROBIN_HASH_MAP     1
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
#define STRING_UTILS_LIBC       3

#define STRING_UTILS_MODE       STRING_UTILS_STL

#include <jstd/basic/stddef.h>
#include <jstd/basic/stdint.h>
#include <jstd/basic/inttypes.h>

#include <unordered_map>

#if USE_JSTD_FLAT16_HASH_MAP
#include <jstd/hashmap/flat16_hash_map.h>
#endif
#if USE_JSTD_ROBIN16_HASH_MAP
#include <jstd/hashmap/robin16_hash_map.h>
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

#include "BenchmarkResult.h"

static std::vector<std::string> dict_words;

static std::string dict_filename;
static bool dict_words_is_ready = false;

static const std::size_t kInitCapacity = 16;

#ifndef _DEBUG
static const std::size_t kIterations = 3000000;
#else
static const std::size_t kIterations = 1000;
#endif

//
// See: https://blog.csdn.net/janekeyzheng/article/details/42419407
//
static const char * header_fields[] = {
    // Request
    "Accept",
    "Accept-Charset",
    "Accept-Encoding",
    "Accept-Language",
    "Authorization",
    "Cache-Control",
    "Connection",
    "Cookie",
    "Content-Length",
    "Content-MD5",
    "Content-Type",
    "Date",  // <-- repeat
    "DNT",
    "From",
    "Front-End-Https",
    "Host",
    "If-Match",
    "If-Modified-Since",
    "If-None-Match",
    "If-Range",
    "If-Unmodified-Since",
    "Max-Forwards",
    "Pragma",
    "Proxy-Authorization",
    "Range",
    "Referer",
    "User-Agent",
    "Upgrade",
    "Via",  // <-- repeat
    "Warning",
    "X-ATT-DeviceId",
    "X-Content-Type-Options",
    "X-Forwarded-For",
    "X-Forwarded-Proto",
    "X-Powered-By",
    "X-Requested-With",
    "X-XSS-Protection",

    // Response
    "Access-Control-Allow-Origin",
    "Accept-Ranges",
    "Age",
    "Allow",
    //"Cache-Control",
    //"Connection",
    "Content-Encoding",
    "Content-Language",
    //"Content-Length",
    "Content-Disposition",
    //"Content-MD5",
    "Content-Range",
    //"Content-Type",
    "Date",  // <-- repeat
    "ETag",
    "Expires",
    "Last-Modified",
    "Link",
    "Location",
    "P3P",
    "Proxy-Authenticate",
    "Refresh",
    "Retry-After",
    "Server",
    "Set-Cookie",
    "Strict-Transport-Security",
    "Trailer",
    "Transfer-Encoding",
    "Vary",
    "Via",  // <-- repeat
    "WWW-Authenticate",
    //"X-Content-Type-Options",
    //"X-Powered-By",
    //"X-XSS-Protection",

    "Last"
};

#if 1

static const size_t kHeaderFieldSize = sizeof(header_fields) / sizeof(char *);

namespace std {

template <>
struct hash<jstd::string_view> {
    typedef jstd::string_view   argument_type;
    typedef std::uint32_t       result_type;

    jstd::string_hash_helper<jstd::string_view, std::uint32_t, jstd::HashFunc_CRC32C> hash_helper_;

    result_type operator()(const argument_type & key) const {
        return hash_helper_.getHashCode(key);
    }
};

} // namespace std

namespace jstd {

template <>
struct hash<jstd::string_view> {
    typedef jstd::string_view   argument_type;
    typedef std::uint32_t       result_type;

    jstd::string_hash_helper<jstd::string_view, std::uint32_t, jstd::HashFunc_CRC32C> hash_helper_;

    result_type operator()(const argument_type & key) const {
        return hash_helper_.getHashCode(key);
    }
};

} // namespace jstd

template <typename Key, typename Value>
void print_error(std::size_t index, const Key & key, const Value & value)
{
    std::string skey   = std::to_string(key);
    std::string svalue = std::to_string(value);

    printf("[%6" PRIuPTR "]: key = \"%s\", value = \"%s\"\n",
           index + 1, skey.c_str(), svalue.c_str());
}

template <>
void print_error(std::size_t index, const std::string & key, const std::string & value)
{
    printf("[%6" PRIuPTR "]: key = \"%s\", value = \"%s\"\n",
           index + 1, key.c_str(), value.c_str());
}

template <>
void print_error(std::size_t index, const jstd::string_view & key, const jstd::string_view & value)
{
    std::string skey   = key.to_string();
    std::string svalue = value.to_string();

    printf("[%6" PRIuPTR "]: key = \"%s\", value = \"%s\"\n",
           index + 1, skey.c_str(), svalue.c_str());
}

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

template <typename Vector>
void copy_and_shuffle_vector(Vector & dest_list, const Vector & src_list) {
    // copy
    dest_list.clear();
    dest_list.reserve(src_list.size());
    for (std::size_t n = 0; n < src_list.size(); n++) {
        dest_list.push_back(src_list[n]);
    }

    // shuffle
    for (std::size_t n = dest_list.size(); n >= 2; n--) {
        std::size_t rnd_idx = std::size_t(jstd::MtRandomGen::nextUInt()) % n;
        std::swap(dest_list[n - 1], dest_list[rnd_idx]);
    }
}

template <>
void copy_and_shuffle_vector(jstd::string_view_array<jstd::string_view, jstd::string_view> & dest_list,
                             const jstd::string_view_array<jstd::string_view, jstd::string_view> & src_list) {
    typedef typename jstd::string_view_array<jstd::string_view, jstd::string_view>::value_type value_type;
    typedef typename std::remove_pointer<value_type>::type element_type;

    // copy
    dest_list.clear();
    dest_list.reserve(src_list.size());
    for (std::size_t n = 0; n < src_list.size(); n++) {
        value_type value = src_list.at(n);
        value_type new_value = new element_type(*value);
        dest_list.push_back(new_value);
    }

    // shuffle
    for (std::size_t n = dest_list.size(); n >= 2; n--) {
        std::size_t rnd_idx = std::size_t(jstd::MtRandomGen::nextUInt()) % n;
        std::swap(dest_list[n - 1], dest_list[rnd_idx]);
    }
}

template <typename T>
void reverse_value(T & value)
{
    std::reverse(value.begin(), value.end());
}

template <>
void reverse_value(std::int32_t & value)
{
    value = ~value;
}

template <>
void reverse_value(std::uint32_t & value)
{
    value = ~value;
}

template <>
void reverse_value(std::int64_t & value)
{
    value = ~value;
}

template <>
void reverse_value(std::uint64_t & value)
{
    value = ~value;
}

template <typename Vector>
void copy_vector_and_reverse_item(Vector & dest_list, const Vector & src_list) {
    typedef typename Vector::value_type value_type;

    // copy
    dest_list.clear();
    dest_list.reserve(src_list.size());
    for (std::size_t n = 0; n < src_list.size(); n++) {
        value_type value = src_list[n];
        reverse_value(value.first);
        dest_list.push_back(value);
    }
}

template <typename Container, typename Vector>
void test_hashmap_find_sequential(const Vector & test_data,
                                  double & elapsedTime, std::size_t & check_sum)
{
    typedef typename Container::const_iterator const_iterator;

    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    Container container(kInitCapacity);
    for (std::size_t i = 0; i < data_length; i++) {
        container.emplace(test_data[i].first, test_data[i].second);
    }

    std::size_t checksum = 0;
    jtest::StopWatch sw;

    sw.start();
    for (std::size_t n = 0; n < repeat_times; n++) {
        for (std::size_t i = 0; i < data_length; i++) {
            const_iterator iter = container.find(test_data[i].first);
            if (iter != container.end()) {
                checksum++;
            }
#ifdef _DEBUG
            else {
                static int err_count = 0;
                err_count++;
                if (err_count < 20) {
                    print_error(i, test_data[i].first, test_data[i].second);
                }
            }
#endif
        }
    }
    sw.stop();

    elapsedTime = sw.getElapsedMillisec();
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_find_random(const Vector & test_data, const Vector & rand_data,
                              double & elapsedTime, std::size_t & check_sum)
{
    typedef typename Container::const_iterator const_iterator;

    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    Container container(kInitCapacity);
    for (std::size_t i = 0; i < data_length; i++) {
        container.emplace(test_data[i].first, test_data[i].second);
    }

    std::size_t checksum = 0;
    jtest::StopWatch sw;

    sw.start();
    for (std::size_t n = 0; n < repeat_times; n++) {
        for (std::size_t i = 0; i < data_length; i++) {
            const_iterator iter = container.find(rand_data[i].first);
            if (iter != container.end()) {
                checksum++;
            }
        }
    }
    sw.stop();

    elapsedTime = sw.getElapsedMillisec();
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_find_failed(const Vector & test_data, const Vector & reverse_data,
                              double & elapsedTime, std::size_t & check_sum)
{
    typedef typename Container::const_iterator const_iterator;

    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    Container container(kInitCapacity);
    for (std::size_t i = 0; i < data_length; i++) {
        container.emplace(test_data[i].first, test_data[i].second);
    }

    std::size_t checksum = 0;
    jtest::StopWatch sw;

    sw.start();
    for (std::size_t n = 0; n < repeat_times; n++) {
        for (std::size_t i = 0; i < data_length; i++) {
            const_iterator iter = container.find(reverse_data[i].first);
            if (iter != container.end()) {
                checksum++;
            }
        }
    }
    sw.stop();

    elapsedTime = sw.getElapsedMillisec();
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_find_empty(const Vector & test_data,
                             double & elapsedTime, std::size_t & check_sum)
{
    typedef typename Container::const_iterator const_iterator;

    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    Container container(kInitCapacity);

    std::size_t checksum = 0;
    jtest::StopWatch sw;

    sw.start();
    for (std::size_t n = 0; n < repeat_times; n++) {
        for (std::size_t i = 0; i < data_length; i++) {
            const_iterator iter = container.find(test_data[i].first);
            if (iter != container.end()) {
                checksum++;
            }
        }
    }
    sw.stop();

    elapsedTime = sw.getElapsedMillisec();
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_insert(const Vector & test_data,
                         double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.insert(std::make_pair(test_data[i].first, test_data[i].second));
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_insert_predicted(const Vector & test_data,
                                   double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        container.reserve(data_length);

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.insert(std::make_pair(test_data[i].first, test_data[i].second));
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_insert_replace(const Vector & test_data,
                                 double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        for (std::size_t i = 0; i < data_length; i++) {
            container.insert(std::make_pair(test_data[i].first, test_data[i].second));
        }

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            std::size_t reverse_idx = data_length - 1 - i;
            container.insert(std::make_pair(test_data[i].first, test_data[reverse_idx].second));
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_emplace(const Vector & test_data,
                          double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_emplace_predicted(const Vector & test_data,
                                    double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        container.reserve(data_length);

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_emplace_replace(const Vector & test_data,
                                  double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            std::size_t reverse_idx = data_length - 1 - i;
            container.emplace(test_data[i].first, test_data[reverse_idx].second);
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_erase_sequential(const Vector & test_data,
                                   double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }
        checksum += container.size();

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.erase(test_data[i].first);
        }
        sw.stop();

        assert(container.size() == 0);
#ifdef _DEBUG
        if (container.size() != 0) {
            static int err_count = 0;
            err_count++;
            if (err_count < 20) {
                printf("container.size() = %" PRIuPTR "\n", container.size());
            }
        }
#endif
        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_erase_random(const Vector & test_data, const Vector & rand_data,
                               double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }
        checksum += container.size();

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.erase(rand_data[i].first);
        }
        sw.stop();

        assert(container.size() == 0);
        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_erase_failed(const Vector & test_data, const Vector & reverse_data,
                               double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    jtest::StopWatch sw;

    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }
        checksum += container.size();

        sw.start();
        for (std::size_t i = 0; i < data_length; i++) {
            container.erase(reverse_data[i].first);
        }
        sw.stop();

        checksum += container.size();
        totalTime += sw.getElapsedMillisec();
    }

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_rehash(const Vector & test_data,
                         double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / 2 / data_length) + 1;
    else
        repeat_times = 0;

    Container container(kInitCapacity);
    for (std::size_t i = 0; i < data_length; i++) {
        container.emplace(test_data[i].first, test_data[i].second);
    }

    std::size_t checksum = 0;
    double totalTime = 0.0;
    std::size_t bucket_counts = 0;
    jtest::StopWatch sw;

    sw.start();
    for (std::size_t n = 0; n < repeat_times; n++) {
        checksum += container.size();
        checksum += container.bucket_count();

        bucket_counts = 128;
        container.rehash(bucket_counts);
        checksum += container.bucket_count();
#ifdef _DEBUG
        static size_t rehash_cnt1 = 0;
        if (rehash_cnt1 < 20) {
            if (container.bucket_count() != bucket_counts) {
                size_t bucket_count = container.bucket_count();
                printf("rehash1():   size = %" PRIuPTR ", buckets = %" PRIuPTR ", bucket_count = %" PRIuPTR "\n",
                        container.size(), bucket_counts, bucket_count);
            }
            rehash_cnt1++;
        }
#endif
        for (std::size_t i = 0; i < 7; i++) {
            bucket_counts *= 2;
            container.rehash(bucket_counts);
            checksum += container.bucket_count();
#ifdef _DEBUG
            static size_t rehash_cnt2 = 0;
            if (rehash_cnt2 < 20) {
                if (container.bucket_count() != bucket_counts) {
                    size_t bucket_count = container.bucket_count();
                    printf("rehash2(%u):   size = %" PRIuPTR ", buckets = %" PRIuPTR ", bucket_count = %" PRIuPTR "\n",
                            (uint32_t)i, container.size(), bucket_counts, bucket_count);
                }
                rehash_cnt2++;
            }
#endif
        }
    }
    sw.stop();

    totalTime = sw.getElapsedMillisec();

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container, typename Vector>
void test_hashmap_rehash2(const Vector & test_data,
                          double & elapsedTime, std::size_t & check_sum)
{
    std::size_t data_length = test_data.size();
    std::size_t repeat_times;
    if (data_length != 0)
        repeat_times = (kIterations / 2 / data_length) + 1;
    else
        repeat_times = 0;

    std::size_t checksum = 0;
    double totalTime = 0.0;
    std::size_t bucket_counts = 0;
    jtest::StopWatch sw;

    sw.start();
    for (std::size_t n = 0; n < repeat_times; n++) {
        Container container(kInitCapacity);
        for (std::size_t i = 0; i < data_length; i++) {
            container.emplace(test_data[i].first, test_data[i].second);
        }
        checksum += container.size();
        checksum += container.bucket_count();

        bucket_counts = 128;
        container.rehash(bucket_counts);
        checksum += container.bucket_count();
#ifdef _DEBUG
        static size_t rehash_cnt1 = 0;
        if (rehash_cnt1 < 20) {
            if (container.bucket_count() != bucket_counts) {
                size_t bucket_count = container.bucket_count();
                printf("rehash1():   size = %" PRIuPTR ", buckets = %" PRIuPTR ", bucket_count = %" PRIuPTR "\n",
                        container.size(), bucket_counts, bucket_count);
            }
            rehash_cnt1++;
        }
#endif
        for (std::size_t i = 0; i < 7; i++) {
            bucket_counts *= 2;
            container.rehash(bucket_counts);
            checksum += container.bucket_count();
#ifdef _DEBUG
            static size_t rehash_cnt2 = 0;
            if (rehash_cnt2 < 20) {
                if (container.bucket_count() != bucket_counts) {
                    size_t bucket_count = container.bucket_count();
                    printf("rehash2(%u):   size = %" PRIuPTR ", buckets = %" PRIuPTR ", bucket_count = %" PRIuPTR "\n",
                            (uint32_t)i, container.size(), bucket_counts, bucket_count);
                }
                rehash_cnt2++;
            }
#endif
        }
    }
    sw.stop();

    totalTime = sw.getElapsedMillisec();

    elapsedTime = totalTime;
    check_sum = checksum;

    print_test_time<Container>(check_sum, elapsedTime);
}

template <typename Container1, typename Container2, typename Vector>
void hashmap_benchmark_simple(const std::string & cat_name,
                              Container1 & container1, Container2 & container2,
                              const Vector & test_data, const Vector & reverse_data,
                              jtest::BenchmarkResult & result)
{
    std::size_t cat_id = result.addCategory(cat_name);

    double elapsedTime1, elapsedTime2;
    std::size_t checksum1, checksum2;

    Vector rand_data;
    copy_and_shuffle_vector(rand_data, test_data);

    //
    // test hashmap<K, V>/find/sequential
    //
    test_hashmap_find_sequential<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_find_sequential<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/find/sequential", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/find/random
    //
    test_hashmap_find_random<Container1, Vector>(test_data, rand_data, elapsedTime1, checksum1);
    test_hashmap_find_random<Container2, Vector>(test_data, rand_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/find/random", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/find/failed
    //
    test_hashmap_find_failed<Container1, Vector>(test_data, reverse_data, elapsedTime1, checksum1);
    test_hashmap_find_failed<Container2, Vector>(test_data, reverse_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/find/failed", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/find/empty
    //
    test_hashmap_find_empty<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_find_empty<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/find/empty", elapsedTime1, checksum1, elapsedTime2, checksum2);

    result.addBlankLine(cat_id);

    //
    // test hashmap<K, V>/insert
    //
    test_hashmap_insert<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_insert<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/insert", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/insert/predicted
    //
    test_hashmap_insert_predicted<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_insert_predicted<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/insert/predicted", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/insert/replace
    //
    test_hashmap_insert_replace<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_insert_replace<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/insert/replace", elapsedTime1, checksum1, elapsedTime2, checksum2);

    result.addBlankLine(cat_id);

    //
    // test hashmap<K, V>/emplace
    //
    test_hashmap_emplace<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_emplace<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/emplace", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/emplace/predicted
    //
    test_hashmap_emplace_predicted<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_emplace_predicted<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/emplace/predicted", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/emplace/replace
    //
    test_hashmap_emplace_replace<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_emplace_replace<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/emplace/replace", elapsedTime1, checksum1, elapsedTime2, checksum2);

    result.addBlankLine(cat_id);

    //
    // test hashmap<K, V>/erase/sequential
    //
    test_hashmap_erase_sequential<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_erase_sequential<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/erase/sequential", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/erase/random
    //
    test_hashmap_erase_random<Container1, Vector>(test_data, rand_data, elapsedTime1, checksum1);
    test_hashmap_erase_random<Container2, Vector>(test_data, rand_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/erase/random", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/erase/failed
    //
    test_hashmap_erase_failed<Container1, Vector>(test_data, reverse_data, elapsedTime1, checksum1);
    test_hashmap_erase_failed<Container2, Vector>(test_data, reverse_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/erase/failed", elapsedTime1, checksum1, elapsedTime2, checksum2);

    result.addBlankLine(cat_id);

    //
    // test hashmap<K, V>/rehash
    //
    test_hashmap_rehash<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_rehash<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/rehash", elapsedTime1, checksum1, elapsedTime2, checksum2);

    //
    // test hashmap<K, V>/rehash2
    //
    test_hashmap_rehash2<Container1, Vector>(test_data, elapsedTime1, checksum1);
    test_hashmap_rehash2<Container2, Vector>(test_data, elapsedTime2, checksum2);

    result.addResult(cat_id, "hash_map<K, V>/rehash2", elapsedTime1, checksum1, elapsedTime2, checksum2);
}

void jstd_flat16_hash_map_benchmark()
{
#if USE_JSTD_FLAT16_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "jstd::flat16_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>    std_map_ii;
        jstd::flat16_hash_map<int, int> jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>        std_map_uu;
        jstd::flat16_hash_map<std::size_t, std::size_t>     jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>    std_map_ss;
        jstd::flat16_hash_map<std::string, std::string> jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>    std_map_svsv;
            jstd::flat16_hash_map<jstd::string_view, jstd::string_view> jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif // USE_JSTD_FLAT16_HASH_MAP
}

void jstd_robin16_hash_map_benchmark()
{
#if USE_JSTD_ROBIN16_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "jstd::robin16_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>        std_map_ii;
        jstd::robin16_hash_map<int, int>    jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>        std_map_uu;
        jstd::robin16_hash_map<std::size_t, std::size_t>    jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>        std_map_ss;
        jstd::robin16_hash_map<std::string, std::string>    jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>     std_map_svsv;
            jstd::robin16_hash_map<jstd::string_view, jstd::string_view> jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif // USE_JSTD_ROBIN16_HASH_MAP
}

void jstd_robin_hash_map_benchmark()
{
#if USE_JSTD_ROBIN_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "jstd::robin_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>    std_map_ii;
        jstd::robin_hash_map<int, int>  jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>    std_map_uu;
        jstd::robin_hash_map<std::size_t, std::size_t>  jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>    std_map_ss;
        jstd::robin_hash_map<std::string, std::string>  jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>    std_map_svsv;
            jstd::robin_hash_map<jstd::string_view, jstd::string_view>  jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif // USE_JSTD_ROBIN_HASH_MAP
}

void ska_flat_hash_map_benchmark()
{
#if USE_SKA_FLAT_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "ska::flat_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>    std_map_ii;
        ska::flat_hash_map<int, int>    jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>    std_map_uu;
        ska::flat_hash_map<std::size_t, std::size_t>    jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>    std_map_ss;
        ska::flat_hash_map<std::string, std::string>    jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>    std_map_svsv;
            ska::flat_hash_map<jstd::string_view, jstd::string_view>    jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif // USE_SKA_FLAT_HASH_MAP
}

void ska_bytell_hash_map_benchmark()
{
#if USE_SKA_BYTELL_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "ska::bytell_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>    std_map_ii;
        ska::bytell_hash_map<int, int>  jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>    std_map_uu;
        ska::bytell_hash_map<std::size_t, std::size_t>  jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>    std_map_ss;
        ska::bytell_hash_map<std::string, std::string>  jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>    std_map_svsv;
            ska::bytell_hash_map<jstd::string_view, jstd::string_view>  jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif
}

void absl_flat_hash_map_benchmark()
{
#if USE_ABSL_FLAT_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "absl::flat_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>    std_map_ii;
        absl::flat_hash_map<int, int>   jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>    std_map_uu;
        absl::flat_hash_map<std::size_t, std::size_t>   jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>    std_map_ss;
        absl::flat_hash_map<std::string, std::string>   jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>    std_map_svsv;
            absl::flat_hash_map<jstd::string_view, jstd::string_view>   jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif // USE_SKA_BYTELL_HASH_MAP
}

void absl_node_hash_map_benchmark()
{
#if USE_ABSL_NODE_HASH_MAP
    jtest::BenchmarkResult test_result;
    test_result.setName("std::unordered_map", "absl::node_hash_map");

    jtest::StopWatch sw;
    sw.start();

    //
    // std::unordered_map<std::string, std::string>
    //
    std::vector<std::pair<std::string, std::string>> test_data_ss;

    if (!dict_words_is_ready) {
        std::string field_str[kHeaderFieldSize];
        std::string index_str[kHeaderFieldSize];
        for (std::size_t i = 0; i < kHeaderFieldSize; i++) {
            test_data_ss.push_back(std::make_pair(std::string(header_fields[i]), std::to_string(i)));
        }
    }
    else {
        for (std::size_t i = 0; i < dict_words.size(); i++) {
            test_data_ss.push_back(std::make_pair(dict_words[i], std::to_string(i)));
        }
    }

    {
        //
        // std::unordered_map<int, int>
        //
        std::vector<std::pair<int, int>> test_data_ii;
        std::vector<std::pair<int, int>> reverse_data_ii;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_ii.push_back(std::make_pair(int(i), int(i + 1)));
        }

        copy_vector_and_reverse_item(reverse_data_ii, test_data_ii);

        std::unordered_map<int, int>    std_map_ii;
        absl::node_hash_map<int, int>   jstd_dict_ii;

        printf(" hash_map<int, int>\n\n");

        hashmap_benchmark_simple("hash_map<int, int>",
                                 std_map_ii, jstd_dict_ii,
                                 test_data_ii, reverse_data_ii, test_result);

        printf("\n");
    }

    {
        //
        // std::unordered_map<size_t, size_t>
        //
        std::vector<std::pair<std::size_t, std::size_t>> test_data_uu;
        std::vector<std::pair<std::size_t, std::size_t>> reverse_data_uu;

        for (std::size_t i = 0; i < test_data_ss.size(); i++) {
            test_data_uu.push_back(std::make_pair(i, i + 1));
        }

        copy_vector_and_reverse_item(reverse_data_uu, test_data_uu);

        std::unordered_map<std::size_t, std::size_t>    std_map_uu;
        absl::node_hash_map<std::size_t, std::size_t>   jstd_dict_uu;

        printf(" hash_map<std::size_t, std::size_t>\n\n");

        hashmap_benchmark_simple("hash_map<std::size_t, std::size_t>",
                                 std_map_uu, jstd_dict_uu,
                                 test_data_uu, reverse_data_uu, test_result);

        printf("\n");
    }

    {
        std::unordered_map<std::string, std::string>    std_map_ss;
        absl::node_hash_map<std::string, std::string>   jstd_dict_ss;

        std::vector<std::pair<std::string, std::string>> reverse_data_ss;
        copy_vector_and_reverse_item(reverse_data_ss, test_data_ss);

        printf(" hash_map<std::string, std::string>\n\n");

        hashmap_benchmark_simple("hash_map<std::string, std::string>",
                                 std_map_ss, jstd_dict_ss,
                                 test_data_ss, reverse_data_ss, test_result);

        printf("\n");

        //
        // Note: Don't remove these braces '{' and '}',
        // because the life cycle of jstd::string_view depends on std::string.
        //
        {
            //
            // std::unordered_map<jstd::string_view, jstd::string_view>
            //
            typedef jstd::string_view_array<jstd::string_view, jstd::string_view> string_view_array_t;
            typedef typename string_view_array_t::element_type                    element_type;

            string_view_array_t test_data_svsv;
            string_view_array_t reverse_data_svsv;

            for (std::size_t i = 0; i < test_data_ss.size(); i++) {
                test_data_svsv.push_back(new element_type(test_data_ss[i].first, test_data_ss[i].second));
            }
            for (std::size_t i = 0; i < reverse_data_ss.size(); i++) {
                reverse_data_svsv.push_back(new element_type(reverse_data_ss[i].first, reverse_data_ss[i].second));
            }

            std::unordered_map<jstd::string_view, jstd::string_view>    std_map_svsv;
            absl::node_hash_map<jstd::string_view, jstd::string_view>   jstd_dict_svsv;

            printf(" hash_map<jstd::string_view, jstd::string_view>\n\n");

            hashmap_benchmark_simple("hash_map<jstd::string_view, jstd::string_view>",
                                     std_map_svsv, jstd_dict_svsv,
                                     test_data_svsv, reverse_data_svsv, test_result);

            printf("\n");
        }
    }

    sw.stop();

    printf("\n");
    test_result.printResult(dict_filename, sw.getElapsedMillisec());
#endif // USE_ABSL_NODE_HASH_MAP
}

bool read_dict_words(const std::string & filename)
{
    bool is_ok = false;
    try {
        std::ifstream dict(filename.c_str());

        if (dict.is_open()) {
            std::string word;
            while (!dict.eof()) {
                char buf[256];
                dict.getline(buf, sizeof(buf));
                word = buf;
                dict_words.push_back(word);
            }

            is_ok = true;
            dict.close();
        }
    }
    catch (const std::exception & ex) {
        std::cout << "read_dict_file() Exception: " << ex.what() << std::endl << std::endl;
        is_ok = false;
    }
    return is_ok;
}

void RandomGenerator_test()
{
    jstd::LibcRandom    random(20200831);
    jstd::MT19937_32    mt_random_32(20200831);
    jstd::MT19937_64    mt_random_64(20200831);
    jstd::RandomGen     RandomGen(20200831);
    jstd::MtRandomGen   mtRandomGen(20200831);
    jstd::MtRandomGen64 mtRandomGen64(20200831);

    int32_t i32 = jstd::RandomGen::nextInt32();
    uint32_t u32 = jstd::MtRandomGen::nextUInt32();
    uint64_t u64 = jstd::MtRandomGen64::nextUInt64();

    printf("\n");
    printf("RandomGen::nextInt32()      = %d\n"
           "MtRandomGen::nextUInt32()   = %u\n"
           "MtRandomGen64::nextUInt64() = %" PRIu64 "\n",
           i32, u32, u64);
    printf("\n");
}

int main(int argc, char * argv[])
{
    jstd::MtRandomGen mtRandomGen(20200831);

    if (argc == 2) {
        std::string filename = argv[1];
        bool read_ok = read_dict_words(filename);
        dict_words_is_ready = read_ok;
        dict_filename = filename;
    }

    jtest::CPU::warm_up(1000);

#if USE_JSTD_FLAT16_HASH_MAP
    if (1)
    {
        jstd_flat16_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if USE_JSTD_ROBIN16_HASH_MAP
    if (1)
    {
        jstd_robin16_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if USE_JSTD_ROBIN_HASH_MAP
    if (1)
    {
        jstd_robin_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if USE_SKA_FLAT_HASH_MAP
    if (1)
    {
        ska_flat_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if USE_SKA_BYTELL_HASH_MAP
    if (1)
    {
        ska_bytell_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if USE_ABSL_FLAT_HASH_MAP
    if (1)
    {
        absl_flat_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if USE_ABSL_NODE_HASH_MAP
    if (1)
    {
        absl_node_hash_map_benchmark();
        jstd::Console::ReadKey();
    }
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
    //jstd::Console::ReadKey();
#endif
    return 0;
}

#else

static inline
uint32_t next_random_u32()
{
#if (RAND_MAX == 0x7FFF)
    uint32_t rnd32 = (((uint32_t)rand() & 0x03) << 30) |
                      ((uint32_t)rand() << 15) |
                       (uint32_t)rand();
#else
    uint32_t rnd32 = ((uint32_t)rand() << 16) | (uint32_t)rand();
#endif
    return rnd32;
}

static inline
uint64_t next_random_u64()
{
#if (RAND_MAX == 0x7FFF)
    uint64_t rnd64 = (((uint64_t)rand() & 0x0F) << 60) |
                      ((uint64_t)rand() << 45) |
                      ((uint64_t)rand() << 30) |
                      ((uint64_t)rand() << 15) |
                       (uint64_t)rand();
#else
    uint64_t rnd64 = ((uint64_t)rand() << 32) | (uint64_t)rand();
#endif
    return rnd64;
}

// Returns a number in the range [min, max) - uint32
template <uint32_t min_val, uint32_t max_val>
static inline
uint32_t get_range_u32(uint32_t num)
{
    if (min_val < max_val) {
        return (min_val + (num % (uint32_t)(max_val - min_val)));
    } else if (min_val > max_val) {
        return (max_val + (num % (uint32_t)(min_val - max_val)));
    } else {
        return num;
    }
}

// Returns a number in the range [min, max) - uint64
template <uint64_t min_val, uint64_t max_val>
static inline
uint64_t get_range_u32(uint64_t num)
{
    if (min_val < max_val) {
        return (min_val + (num % (uint64_t)(max_val - min_val)));
    } else if (min_val > max_val) {
        return (max_val + (num % (uint64_t)(min_val - max_val)));
    } else {
        return num;
    }
}

void IntegalHash_test()
{
    jstd::v1::hashes::IntegalHash<std::uint32_t> integalHasher32;
    jstd::v1::hashes::IntegalHash<std::uint64_t> integalHasher64;

    printf("hash::IntegalHash(uint32_t) sequential\n\n");
    for (std::uint32_t i = 0; i < 16; i++) {
        std::uint32_t hash32 = (std::uint32_t)integalHasher32(i);
        printf("value = %-10u, hash_code = %-10u (0x%08X), \n",
               i, hash32, hash32);
    }
    printf("\n");

    printf("hash::IntegalHash(uint32_t) random\n\n");
    for (std::uint32_t i = 0; i < 16; i++) {
        std::uint32_t value = next_random_u32();
        std::uint32_t hash32 = (std::uint32_t)integalHasher32(value);
        printf("value = %-10u, hash_code = %-10u (0x%08X), \n",
               value, hash32, hash32);
    }
    printf("\n");

    printf("hash::IntegalHash(uint64_t) sequential\n\n");
    for (std::uint64_t i = 0; i < 16; i++) {
        std::uint64_t hash64 = integalHasher64(i);
        printf("value = %-20" PRIu64 ", hash_code = %-20" PRIu64 " (0x%08X%08X)\n",
               i, hash64,
               (std::uint32_t)(hash64 >> 32),
               (std::uint32_t)(hash64 & 0xFFFFFFFFul));
    }
    printf("\n");

    printf("hash::IntegalHash(uint64_t) random\n\n");
    for (std::uint64_t i = 0; i < 16; i++) {
        std::uint64_t value = next_random_u64();
        std::uint64_t hash64 = integalHasher64(value);
        printf("value = %-20" PRIu64 ", hash_code = %-20" PRIu64 " (0x%08X%08X)\n",
               value, hash64,
               (std::uint32_t)(hash64 >> 32),
               (std::uint32_t)(hash64 & 0xFFFFFFFFul));
    }
    printf("\n");
}

template <typename PairType>
void print_node(const PairType & node) {
    std::cout << "[" << node.first << "] = " << node.second << '\n';
};

template <typename PairType>
void print_result(const PairType & pair) {
    std::cout << (pair.second ? "inserted: " : "ignored:  ");
    print_node(*pair.first);
};

void flat16_hash_map_int_int_test()
{
    typedef jstd::flat16_hash_map<int, int> hash_map_t;
    typedef typename hash_map_t::slot_type      slot_type;
    typedef typename hash_map_t::group_type     group_type;

    hash_map_t flat_hash_map;

    printf("flat_hash_map.groups()    = %p\n", flat_hash_map.groups());
    printf("sizeof(group_type)        = %u\n\n", (uint32_t)sizeof(group_type));

    printf("flat_hash_map.slot_type() = %p\n", flat_hash_map.slots());
    printf("sizeof(slot_type)         = %u\n\n", (uint32_t)sizeof(slot_type));

    flat_hash_map.insert(std::make_pair(1, 111));
    flat_hash_map.insert(std::make_pair(1, 999));

    flat_hash_map.emplace(std::make_pair(2, 222));
    flat_hash_map.emplace(std::make_pair(2, 999));

    auto iter = flat_hash_map.find(1);
    if (iter != flat_hash_map.end()) {
        printf("Found, key = %d, value = %d\n\n", iter->first, iter->second);
    } else {
        printf("Not found, key = %d\n\n", 1);
    }

    flat_hash_map.erase(2);

    iter = flat_hash_map.find(2);
    if (iter != flat_hash_map.end()) {
        printf("Found, key = %d, value = %d\n\n", iter->first, iter->second);
    } else {
        printf("Not found, key = %d\n\n", 2);
    }
}

void flat16_hash_map_int64_string_test()
{
    typedef jstd::flat16_hash_map<int64_t, std::string> hash_map_t;
    typedef typename hash_map_t::slot_type      slot_type;
    typedef typename hash_map_t::group_type     group_type;

    hash_map_t flat_hash_map;

    printf("flat_hash_map.groups()     = %p\n", flat_hash_map.groups());
    printf("sizeof(group_type)        = %u\n\n", (uint32_t)sizeof(group_type));

    printf("flat_hash_map.slot_type() = %p\n", flat_hash_map.slots());
    printf("sizeof(slot_type)         = %u\n\n", (uint32_t)sizeof(slot_type));

    flat_hash_map.insert(std::make_pair(0, "abc"));
    flat_hash_map.insert(std::make_pair(0, "ABC"));

    flat_hash_map.emplace(std::make_pair(1, "xyz"));
    flat_hash_map.emplace(std::make_pair(1, "XYZ"));

    flat_hash_map.emplace(2, "in-place");
    flat_hash_map.emplace(std::piecewise_construct,
                          std::forward_as_tuple(3),
                          std::forward_as_tuple(10, 'c'));

    auto iter = flat_hash_map.find(0);
    if (iter != flat_hash_map.end()) {
        printf("Found, key = %d, value = \"%s\"\n\n", (int)iter->first, iter->second.c_str());
    } else {
        printf("Not found, key = %d\n\n", 0);
    }

    flat_hash_map.erase(1);

    iter = flat_hash_map.find(1);
    if (iter != flat_hash_map.end()) {
        printf("Found, key = %d, value = \"%s\"\n\n", (int)iter->first, iter->second.c_str());
    } else {
        printf("Not found, key = %d\n\n", 1);
    }
}

void flat16_hash_map_string_string_test()
{
    typedef jstd::flat16_hash_map<std::string, std::string> hash_map_t;
    typedef typename hash_map_t::slot_type      slot_type;
    typedef typename hash_map_t::group_type     group_type;

    hash_map_t flat_hash_map;

    printf("flat_hash_map.groups()    = %p\n", flat_hash_map.groups());
    printf("sizeof(group_type)        = %u\n\n", (uint32_t)sizeof(group_type));

    printf("flat_hash_map.slot_type() = %p\n", flat_hash_map.slots());
    printf("sizeof(slot_type)         = %u\n\n", (uint32_t)sizeof(slot_type));

#if 0
    print_result( flat_hash_map.insert("a", "a") );
    print_result( flat_hash_map.insert("b", "abcd") );
    print_result( flat_hash_map.insert("c", "Won't be inserted") );
#endif

    print_result( flat_hash_map.emplace("a0", "a") );
    print_result( flat_hash_map.emplace("b0", "abcd") );
    print_result( flat_hash_map.emplace("c0", 10, 'c') );
    print_result( flat_hash_map.emplace("c0", "Won't be inserted") );

    print_result( flat_hash_map.emplace("d0", "in-place") );
    printf("\n");
    print_result( flat_hash_map.emplace(std::piecewise_construct,
                                        std::forward_as_tuple("e0"),
                                        std::forward_as_tuple(10, 'd')) );
    printf("\n");

    print_result( flat_hash_map.insert_or_assign("f0", "in-place") );
    printf("\n");

    auto iter = flat_hash_map.find("b0");
    if (iter != flat_hash_map.end()) {
        printf("Found, key = \"%s\", value = \"%s\"\n\n", iter->first.c_str(), iter->second.c_str());
    } else {
        printf("Not found, key = \"%s\"\n\n", "b0");
    }

    flat_hash_map.erase("c0");

    iter = flat_hash_map.find("c0");
    if (iter != flat_hash_map.end()) {
        printf("Found, key = \"%s\", value = \"%s\"\n\n", iter->first.c_str(), iter->second.c_str());
    } else {
        printf("Not found, key = \"%s\"\n\n", "c0");
    }
}

int main(int argc, char * argv[])
{
    // Random number seed
    srand((unsigned int)time(NULL));

    if (0) { IntegalHash_test(); }
    if (1) { flat16_hash_map_int_int_test(); }
    if (1) { flat16_hash_map_int64_string_test(); }
    if (1) { flat16_hash_map_string_string_test(); }

    printf("sizeof(jstd::flat_hash_map<K, V>) = %u\n\n", (uint32_t)sizeof(jstd::flat16_hash_map<int, int>));

    return 0;
}

#endif
