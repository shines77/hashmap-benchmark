
#ifndef JTEST_BENCHMARK_H
#define JTEST_BENCHMARK_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jstd/basic/stdint.h"
#include "jstd/basic/stdsize.h"
#include "jstd/basic/inttypes.h"

#include <stdio.h>
#include <assert.h>

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <type_traits>

namespace jtest {

class BenchmarkBase {
public:
    using size_type = std::size_t;

protected:
    size_type   id_;
    std::string name_;
    std::string label_;

public:
    BenchmarkBase(size_type id = size_type(-1)) : id_(id) {}
    BenchmarkBase(size_type id, const std::string & name, const std::string & label = "")
        : id_(id), name_(name), label_(label) {}

    BenchmarkBase(const BenchmarkBase & src)
        : id_(src.id()),
          name_(src.name()), label_(src.label()) {}

    BenchmarkBase(BenchmarkBase && src)
        : id_(src.id()),
          name_(std::move(src.name())), label_(std::move(src.label())) {}

    ~BenchmarkBase() {}

    size_type id() const {
        return this->id_;
    }

    void setId(size_type id) {
        this->id_ = id;
    }

    std::string & name() {
        return this->name_;
    }

    const std::string & name() const {
        return this->name_;
    }

    std::string & label() {
        return this->label_;
    }

    const std::string & label() const {
        return this->label_;
    }

    void setName(const std::string & name) {
        this->name_ = name;
    }

    void setLabel(const std::string & label) {
        this->label_ = label;
    }

    void setName(const std::string & name, const std::string & label) {
        this->name_ = name;
        this->label_ = label;
    }

    void swap(BenchmarkBase & other) noexcept {
        if (std::addressof(other) != this) {
            std::swap(this->id_,    other.id_);
            std::swap(this->name_,  other.name_);
            std::swap(this->label_, other.label_);
        }
    }
};

namespace detail {

template <bool IsPointer, typename ValueT, typename ElementT,
          typename std::enable_if<IsPointer>::type * = nullptr,
          typename ... Args>
ValueT construct_if(Args && ... args)
{
    return new ElementT(std::forward<Args>(args)...);
}

template <bool IsPointer, typename ValueT, typename ElementT,
          typename std::enable_if<!IsPointer>::type * = nullptr,
          typename ... Args>
ValueT construct_if(Args && ... args)
{
    return std::move(ElementT(std::forward<Args>(args)...));
}

template <bool IsPointer, typename VectorT,
          typename std::enable_if<IsPointer>::type * = nullptr>
void destroy_if(VectorT & vector)
{
    using value_type = typename VectorT::value_type;
    for (std::size_t i = 0; i < vector.size(); i++) {
        value_type element = vector[i];
        if (element != nullptr) {
            delete element;
            vector[i] = nullptr;
        }
    }
    vector.clear();
}

template <bool IsPointer, typename VectorT,
          typename std::enable_if<!IsPointer>::type * = nullptr>
void destroy_if(VectorT & vector)
{
    JSTD_UNUSED(vector);
    /* Do nothing !! */
}

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

std::string formatMsTime(double fMillisec)
{
    char time_buf[256];

    if (fMillisec >= 1000.0 * 60.0 * 30.0) {
        snprintf(time_buf, sizeof(time_buf), "%7.2f Min", fMillisec / (60.0 * 1000.0));
    }
    else if (fMillisec >= 1000.0 * 10.0) {
        snprintf(time_buf, sizeof(time_buf), "%7.2f Sec", fMillisec / 1000.0);
    }
    else if (fMillisec >= 1.0 * 1.0) {
        snprintf(time_buf, sizeof(time_buf), "%7.2f ms ", fMillisec);
    }
    else if (fMillisec >= 0.001 * 10.0) {
        snprintf(time_buf, sizeof(time_buf), "%7.2f us ", fMillisec * 1000.0);
    }
    else {
        snprintf(time_buf, sizeof(time_buf), "%7.2f ns ", fMillisec * 1000000.0);
    }

    return std::string(time_buf);
}

} // namespace detail

template <typename Key, typename Value, bool ValueIsPointer = std::is_pointer<Value>::value>
class ArrayHashmap {
public:
    using size_type = std::size_t;

    using key_type = Key;
    using value_type = Value;
    using element_type = typename std::remove_pointer<Value>::type;
    using ident_type = size_type;

    using array_type = std::vector<value_type>;
    using hashmap_type = std::unordered_map<key_type, ident_type>;

    using iterator = typename hashmap_type::iterator;
    using const_iterator = typename hashmap_type::const_iterator;

    static constexpr const size_type npos = static_cast<size_type>(-1);
    static constexpr const bool kValueIsPointer = std::is_pointer<value_type>::value;

protected:
    array_type   array_;
    hashmap_type hashmap_;

    void destroy() {
        detail::destroy_if<kValueIsPointer>(array_);
    }

    static value_type s_empty_value;

public:
    ArrayHashmap() = default;
    ~ArrayHashmap() {
        destroy();
    }

    size_type size() const { return array_.size(); }
    size_type max_id() const {
        return static_cast<size_type>(array_.size() - 1);
    }

    element_type & get(size_type index) {
        assert(index< array_.size());
        return array_[index];
    }

    const element_type & get(size_type index) const {
        assert(index< array_.size());
        return array_[index];
    }

    void set(size_type index, const element_type & value) {
        assert(index< array_.size());
        array_[index] = value;
    }

    void set(size_type index, element_type && value) {
        assert(index< array_.size());
        array_[index] = std::move(value);
    }

    void clear() {
        array_.clear();
    }

    void clearMap() {
        hashmap_.clear();
    }

    size_type registerKey(const key_type & key) {
        auto iter = hashmap_.find(key);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return static_cast<size_type>(id);
        } else {
            ident_type id = static_cast<ident_type>(array_.size());

            auto result = hashmap_.emplace(key, id);
            if (result.second)
                return id;
            else
                return npos;
        }
    }

    bool isExists(const key_type & key) const {
        auto iter = hashmap_.find(key);
        return (iter != hashmap_.cend());
    }

    iterator find(const key_type & key) {
        return hashmap_.find(key);
    }

    const_iterator find(const key_type & key) const {
        return hashmap_.find(key);
    }

    value_type & query(const key_type & key) {
        auto iter = hashmap_.find(key);
        if (iter != hashmap_.end()) {
            return get(iter->second);
        } else {
            return s_empty_value;
        }
    }

    const value_type & query(const key_type & key) const {
        return const_cast<ArrayHashmap *>(this)->query((key));
    }

    std::pair<size_type, bool> append(const key_type & name, const key_type & label) {
        auto iter = hashmap_.find(name);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return { static_cast<size_type>(id), true };
        } else {
            ident_type id = static_cast<ident_type>(array_.size());
            value_type value = detail::construct_if<kValueIsPointer, value_type, element_type>(id, name, label);
            if (kValueIsPointer)
                array_.push_back(value);
            else
                array_.push_back(std::move(value));

            auto result = hashmap_.emplace(name, id);
            if (result.second)
                return { id, false };
            else
                return { npos, false };
        }
    }

    std::pair<size_type, bool> append(key_type && name, key_type && label) {
        auto iter = hashmap_.find(name);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return { static_cast<size_type>(id), true };
        } else {
            ident_type id = static_cast<ident_type>(array_.size());
            value_type value = detail::construct_if<kValueIsPointer, value_type, element_type>(
                id, std::forward<key_type>(name), std::forward<key_type>(label));
            if (kValueIsPointer)
                array_.push_back(value);
            else
                array_.push_back(std::move(value));

            auto result = hashmap_.emplace(name, id);
            if (result.second)
                return { id, false };
            else
                return { npos, false };
        }
    }
};

template <typename Key, typename Value, bool kValueIsPointer /*= false*/>
#ifdef _MSC_VER
__declspec(selectany)
#endif
typename ArrayHashmap<Key, Value, kValueIsPointer>::value_type
ArrayHashmap<Key, Value, kValueIsPointer>::s_empty_value;

template <typename Key, typename Value>
class ArrayHashmap<Key, Value, true> {
public:
    using size_type = std::size_t;

    using key_type = Key;
    using value_type = Value;
    using element_type = typename std::remove_pointer<Value>::type;
    using ident_type = size_type;

    using array_type = std::vector<value_type>;
    using hashmap_type = std::unordered_map<key_type, ident_type>;

    using iterator = typename hashmap_type::iterator;
    using const_iterator = typename hashmap_type::const_iterator;

    static constexpr const size_type npos = static_cast<size_type>(-1);
    static constexpr const bool kValueIsPointer = std::is_pointer<value_type>::value;

protected:
    array_type   array_;
    hashmap_type hashmap_;

    void destroy() {
        detail::destroy_if<kValueIsPointer>(array_);
    }

public:
    ArrayHashmap() = default;
    ~ArrayHashmap() {
        destroy();
    }

    size_type size() const { return array_.size(); }
    size_type max_id() const {
        return static_cast<size_type>(array_.size() - 1);
    }

    element_type * get(size_type index) {
        assert(index< array_.size());
        return array_[index];
    }

    const element_type * get(size_type index) const {
        assert(index< array_.size());
        return array_[index];
    }

    void set(size_type index, const value_type & value) {
        assert(index< array_.size());
        array_[index] = value;
    }

    void set(size_type index, value_type && value) {
        assert(index< array_.size());
        array_[index] = std::move(value);
    }

    void clear() {
        array_.clear();
    }

    void clearMap() {
        hashmap_.clear();
    }

    size_type registerKey(const key_type & key) {
        auto iter = hashmap_.find(key);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return static_cast<size_type>(id);
        } else {
            ident_type id = static_cast<ident_type>(array_.size());

            auto result = hashmap_.emplace(key, id);
            if (result.second)
                return id;
            else
                return npos;
        }
    }

    bool isExists(const key_type & key) const {
        auto iter = hashmap_.find(key);
        return (iter != hashmap_.cend());
    }

    iterator find(const key_type & key) {
        return hashmap_.find(key);
    }

    const_iterator find(const key_type & key) const {
        return hashmap_.find(key);
    }

    value_type query(const key_type & key) {
        auto iter = hashmap_.find(key);
        if (iter != hashmap_.end()) {
            return get(iter->second);
        } else {
            return nullptr;
        }
    }

    const value_type query(const key_type & key) const {
        return const_cast<ArrayHashmap *>(this)->query((key));
    }

    std::pair<size_type, bool> append(const key_type & name, const key_type & label) {
        auto iter = hashmap_.find(name);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return { static_cast<size_type>(id), true };
        } else {
            ident_type id = static_cast<ident_type>(array_.size());
            value_type value = detail::construct_if<kValueIsPointer, value_type, element_type>(id, name, label);
            array_.push_back(value);

            auto result = hashmap_.emplace(name, id);
            if (result.second)
                return { id, false };
            else
                return { npos, false };
        }
    }

    std::pair<size_type, bool> append(key_type && name, key_type && label) {
        auto iter = hashmap_.find(name);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return { static_cast<size_type>(id), true };
        } else {
            ident_type id = static_cast<ident_type>(array_.size());
            value_type value = detail::construct_if<kValueIsPointer, value_type, element_type>(
                std::forward<key_type>(name), std::forward<key_type>(label));
            array_.push_back(value);

            auto result = hashmap_.emplace(name, id);
            if (result.second)
                return { id, false };
            else
                return { npos, false };
        }
    }

    std::pair<size_type, bool> append(const key_type & name, element_type * element) {
        auto iter = hashmap_.find(name);
        if (iter != hashmap_.end()) {
            ident_type id = iter->second;
            return { static_cast<size_type>(id), true };
        } else {
            ident_type id = static_cast<ident_type>(array_.size());
            array_.push_back(reinterpret_cast<value_type>(element));

            auto result = hashmap_.emplace(name, id);
            if (result.second)
                return { id, false };
            else
                return { npos, false };
        }
    }
};

//
// An individual benchmark result.
//
struct BenchmarkResult {
    typedef std::size_t size_type;

    size_type   id;
    std::string name;
    std::string hashmap_name;
    std::string blueprint_name;
    size_type   benchmark_id;
    double      average_time;
    double      elasped_times[RUN_COUNT];
    size_type   checksum;

    BenchmarkResult() noexcept : benchmark_id(size_type(-1)), average_time(0.0), checksum(0) {
        for (size_type i = 0; i < RUN_COUNT; i++) {
            elasped_times[i] = 0.0;
        }
    }

    BenchmarkResult(const std::string & name,
                    const std::string & hashmap_name,
                    const std::string & blueprint_name,
                    size_type benchmark_id, double average_time, size_type checksum)
        : name(name), hashmap_name(hashmap_name),
          blueprint_name(blueprint_name),
          benchmark_id(benchmark_id),
          average_time(average_time), checksum(checksum) {
        for (size_type i = 0; i < RUN_COUNT; i++) {
            elasped_times[i] = 0.0;
        }
    }

    BenchmarkResult(const BenchmarkResult & src)
        : name(src.name), hashmap_name(src.hashmap_name),
          blueprint_name(src.blueprint_name),
          benchmark_id(src.benchmark_id),
          average_time(src.average_time), checksum(src.checksum) {
        for (size_type i = 0; i < RUN_COUNT; i++) {
            elasped_times[i] = src.elasped_times[i];
        }
    }

    BenchmarkResult(BenchmarkResult && src)
        : name(std::move(src.name)),
          hashmap_name(std::move(src.hashmap_name)),
          blueprint_name(std::move(src.blueprint_name)),
          benchmark_id(src.benchmark_id),
          average_time(src.average_time), checksum(src.checksum) {
        for (size_type i = 0; i < RUN_COUNT; i++) {
            elasped_times[i] = src.elasped_times[i];
        }
    }

    void setElapsedTimes(double elasped_times[RUN_COUNT]) {
        for (size_type i = 0; i < RUN_COUNT; i++) {
            this->elasped_times[i] = elasped_times[i];
        }
    }

    void swap(BenchmarkResult & other) noexcept {
        if (std::addressof(other) != this) {
            std::swap(this->id,             other.id);
            std::swap(this->name,           other.name);
            std::swap(this->hashmap_name,   other.hashmap_name);
            std::swap(this->blueprint_name, other.blueprint_name);
            std::swap(this->benchmark_id,   other.benchmark_id);
            std::swap(this->average_time,   other.average_time);
            std::swap(this->checksum,       other.checksum);
            for (size_type i = 0; i < RUN_COUNT; i++) {
                std::swap(this->elasped_times[i], other.elasped_times[i]);
            }
        }
    }
};

class BenchmarkCategory : public BenchmarkBase,
                          public ArrayHashmap<std::size_t, BenchmarkResult *> {
public:
    typedef std::size_t size_type;

private:
    size_type benchmark_id_;

    void destroy() {
        /*
        for (size_type i = 0; i < results_.size(); i++) {
            Result * result = results_[i];
            if (result != nullptr) {
                delete result;
                results_[i] = nullptr;
            }
        }
        results_.clear();
        */
    }

public:
    BenchmarkCategory(size_type id = size_type(-1)) : BenchmarkBase(id), ArrayHashmap(), benchmark_id_(size_type(-1)) {}

    BenchmarkCategory(size_type id, const std::string & name, const std::string & label)
        : BenchmarkBase(id, name, label), ArrayHashmap(), benchmark_id_(size_type(-1)) {}

    ~BenchmarkCategory() {
        destroy();
    }

    size_type getBenchmarkId() const {
        return benchmark_id_;
    }

    void setBenchmarkId(size_type benchmark_id) {
        benchmark_id_ = benchmark_id;
    }

    BenchmarkResult * getResult(size_type index) {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    const BenchmarkResult * getResult(size_type index) const {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    BenchmarkResult * getResultById(size_type benchmark_id) {
        for (size_type i = 0; i < array_.size(); i++) {
            BenchmarkResult * reslut = array_[i];
            if (reslut->benchmark_id == benchmark_id) {
                return reslut;
            }
        }
        return nullptr;
    }

    const BenchmarkResult * getResultById(size_type benchmark_id) const {
        return const_cast<BenchmarkCategory *>(this)->getResultById((benchmark_id));
    }

    BenchmarkResult * addResult(const std::string & hashmap_name,
                                const std::string & blueprint_name,
                                size_type benchmark_id,
                                double average_time,
                                double elasped_times[RUN_COUNT],
                                size_type checksum) {
        BenchmarkResult * result = new BenchmarkResult("", hashmap_name, blueprint_name,
                                                       benchmark_id, average_time, checksum);
        result->setElapsedTimes(elasped_times);
        auto info = append(benchmark_id, result);
        if (info.first != npos) {
            auto _result = get(info.first);
            return _result;
        } else {
            delete result;
            return nullptr;
        }
    }
};

class BenchmarkHashmap : public BenchmarkBase,
                         public ArrayHashmap<std::string, BenchmarkCategory *> {
public:
    typedef std::size_t size_type;

    BenchmarkHashmap(size_type id = size_type(-1)) : BenchmarkBase(id), ArrayHashmap() {}

    BenchmarkHashmap(size_type id, const std::string & name, const std::string & label)
        : BenchmarkBase(id, name, label), ArrayHashmap() {}

    ~BenchmarkHashmap() {
        destroy();
    }

    BenchmarkCategory * getCategory(size_type index) {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    const BenchmarkCategory * getCategory(size_type index) const {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    BenchmarkCategory * getCategoryById(size_type benchmark_id) {
        for (size_type i = 0; i < array_.size(); i++) {
            auto category = array_[i];
            if (category->getBenchmarkId() == benchmark_id) {
                return category;
            }
        }
        return nullptr;
    }

    const BenchmarkCategory * getCategoryById(size_type benchmark_id) const {
        return const_cast<BenchmarkHashmap *>(this)->getCategoryById((benchmark_id));
    }

    BenchmarkCategory * getCategoryByName(const std::string & name) {
        BenchmarkCategory * category = query(name);
        return category;
    }

    const BenchmarkCategory * getCategoryByName(const std::string & name) const {
        const BenchmarkCategory * category = query(name);
        return category;
    }

    BenchmarkCategory * addCategory(size_type benchmark_id,
                                    const std::string & name,
                                    const std::string & label) {
        auto result = append(name, label);
        if (result.first != npos) {
            auto category = get(result.first);
            category->setBenchmarkId(benchmark_id);
            return category;
        } else {
            return nullptr;
        }
    }
};

class BenchmarkBluePrint : public BenchmarkBase,
                           public ArrayHashmap<std::string, BenchmarkHashmap *> {
public:
    typedef std::size_t size_type;

private:
    size_type data_size_;
    size_type element_size_;

public:
    BenchmarkBluePrint(size_type id = size_type(-1)) : BenchmarkBase(id), ArrayHashmap(),
        data_size_(0), element_size_(0) {}

    BenchmarkBluePrint(size_type id, const std::string & name, const std::string & label)
        : BenchmarkBase(id, name, label), ArrayHashmap(),
          data_size_(0), element_size_(0) {}

    ~BenchmarkBluePrint() {
        destroy();
    }

    size_type getDataSize() const {
        return data_size_;
    }

    size_type getElementSize() const {
        return element_size_;
    }

    void setDataSize(size_type data_size) {
        data_size_ = data_size;
    }

    void setElementSize(size_type element_size) {
        element_size_ = element_size;
    }

    BenchmarkHashmap * getHashmap(size_type index) {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    const BenchmarkHashmap * getHashmap(size_type index) const {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    BenchmarkHashmap * getHashmap(const std::string & name) {
        BenchmarkHashmap * hashmap = query(name);
        return hashmap;
    }

    const BenchmarkHashmap * getHashmap(const std::string & name) const {
        const BenchmarkHashmap * hashmap = query(name);
        return hashmap;
    }

    BenchmarkHashmap * addHashmap(const std::string & name, const std::string & label) {
        auto result = append(name, label);
        if (result.first != npos) {
            return get(result.first);
        } else {
            return nullptr;
        }
    }
};

class BenchmarkResults : public BenchmarkBase,
                         public ArrayHashmap<std::string, BenchmarkBluePrint *> {
public:
    typedef std::size_t size_type;

private:
    /*
    void destroy() {
        for (size_type i = 0; i < size(); i++) {
            BenchmarkBluePrint * blueprint = get(i);
            if (blueprint != nullptr) {
                delete blueprint;
                set(i, nullptr);
            }
        }
        clear();
    }
    //*/

public:
    BenchmarkResults(size_type id = size_type(-1)) : BenchmarkBase(id), ArrayHashmap() {}

    BenchmarkResults(size_type id, const std::string & name, const std::string & label)
        : BenchmarkBase(id, name, label), ArrayHashmap() {}

    ~BenchmarkResults() {
        destroy();
    }

    BenchmarkBluePrint * getBluePrint(size_type index) {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    const BenchmarkBluePrint * getBluePrint(size_type index) const {
        if (index < size())
            return get(index);
        else
            return nullptr;
    }

    BenchmarkBluePrint * getBluePrint(const std::string & name) {
        BenchmarkBluePrint * blueprint = query(name);
        return blueprint;
    }

    const BenchmarkBluePrint * getBluePrint(const std::string & name) const {
        const BenchmarkBluePrint * blueprint = query(name);
        return blueprint;
    }

    template <typename BluePrint>
    BenchmarkBluePrint * addBluePrint(const std::string & name, const std::string & label) {
        auto result = append(name, label);
        if (result.first != npos) {
            typedef typename BluePrint::element_type element_type;
            auto blueprint = get(result.first);
            blueprint->setDataSize(BluePrint::get_data_size());
            blueprint->setElementSize(sizeof(element_type));
            return blueprint;
        } else {
            return nullptr;
        }
    }

    template <typename BluePrint>
    BenchmarkBluePrint * addBlankLine() {
        return addBluePrint<BluePrint>("_blank", "_blank");
    }

    std::string buildTableHeader(const BenchmarkBluePrint * blueprint, size_type & short_name_len) const {
        std::vector<std::string> bench_short_names;

        if (blueprint != nullptr) {
            const BenchmarkHashmap * hashmap = blueprint->getHashmap(0);
            if (hashmap != nullptr) {
                size_type category_count = hashmap->size();
                for (size_type category_id = 0; category_id < category_count; category_id++) {
                    const BenchmarkCategory * category = hashmap->getCategory(category_id);
                    if (category != nullptr) {
                        std::string short_name = get_benchmark_short_name(category->getBenchmarkId());
                        bench_short_names.push_back(short_name);
                    }
                }
            }
        }

        short_name_len = bench_short_names.size();

        std::string strHeader;
        // Line 1
        strHeader = "-----------------------------";
        for (size_type i = 0; i < short_name_len; i++) {
            strHeader += "-------------";
        }
        strHeader += "\n";
        // Line 2
        strHeader += " Hashmap                    |";
        for (size_type i = 0; i < short_name_len; i++) {
            strHeader += bench_short_names[i];
            strHeader += "|";
        }
        strHeader += "\n";
        // Line 3
        strHeader += "----------------------------+";
        for (size_type i = 0; i < short_name_len; i++) {
            strHeader += "------------+";
        }
        strHeader += "\n";
        return strHeader;
    }

    std::string buildTableFooter(size_type short_name_len) const {
        std::string strHeader;
        strHeader = "-----------------------------";
        for (size_type i = 0; i < short_name_len; i++) {
            strHeader += "-------------";
        }
        strHeader += "\n";
        return strHeader;
    }

    /**************************************************************************************************************************************
       BluePrint: uint32_uint32_murmur          Data size: 123,456,789        Element size: 16 bytes
      -------------------------------------------------------------------------------------------------------------------------------------
       Hashmap                    | find.exist |  find.non  | insert.non |insert.exist|   replace  | erase.exist|  erase.non |  iteration |
      ----------------------------+------------+------------+------------+------------+------------+------------+------------+------------+
       std::unordered_map         | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms |
       jstd::robin_hash_map       | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms |
       jstd::cluster_flat_map     | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms | 1234.00 ms |
      -------------------------------------------------------------------------------------------------------------------------------------
     **************************************************************************************************************************************/
    void printResults() const {
        for (size_type blueprintId = 0; blueprintId < size(); blueprintId++) {
            const BenchmarkBluePrint * blueprint = getBluePrint(blueprintId);
            if (blueprint != nullptr) {
                std::size_t dataSize = blueprint->getDataSize();
                size_type elementSize = blueprint->getElementSize();
                printf("\n");
                printf(" BluePrint: %-25s (%" PRIuPTR ")     Data size: %-11s    Element size: %" PRIuPTR " bytes\n",
                       blueprint->name().c_str(), blueprint->id(), detail::format_integer<3>(dataSize).c_str(), elementSize);
                printf("\n");

                // Table header
                // "-------------------------------------------------------------------------------------------------------------------------------------\n"
                // " Hashmap                    | find.exist |  find.non  | insert.non |insert.exist|   replace  | erase.exist|  erase.non |  iteration |\n"
                // "----------------------------+------------+------------+------------+------------+------------+------------+------------+------------+\n"
                size_type short_name_len = 0;
                std::string strHeader = buildTableHeader(blueprint, short_name_len);
                printf("%s", strHeader.c_str());

                size_type hashmap_count = blueprint->size();
                for (size_type hashmap_id = 0; hashmap_id < hashmap_count; hashmap_id++) {
                    const BenchmarkHashmap * hashmap = blueprint->getHashmap(hashmap_id);
                    if (hashmap != nullptr) {
                        printf(" %-26s |", hashmap->name().c_str());
                        size_type category_count = hashmap->size();
                        for (size_type category_id = 0; category_id < category_count; category_id++) {
                            const BenchmarkCategory * category = hashmap->getCategory(category_id);
                            if (category != nullptr) {
                                size_type rusult_count = category->size();
                                for (size_type rusult_id = 0; rusult_id < rusult_count; rusult_id++) {
                                    const BenchmarkResult * result = category->getResult(rusult_id);
                                    if (result != nullptr) {
                                        printf(" %11s|", detail::formatMsTime(result->average_time).c_str());
                                    }
                                }
                            }
                        }
                        printf("\n");
                    }
                }

                // Table footer
                // "-------------------------------------------------------------------------------------------------------------------------------------\n"
                std::string strFooter = buildTableFooter(short_name_len);
                printf("%s", strFooter.c_str());
            }
        }

        printf("\n");
    }
};

} // namespace jtest

#endif // JTEST_BENCHMARK_H
