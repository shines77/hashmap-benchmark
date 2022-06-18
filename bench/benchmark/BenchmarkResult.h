
#ifndef JTEST_BENCHMARK_H
#define JTEST_BENCHMARK_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jstd/basic/stdint.h"
#include "jstd/basic/stdsize.h"
#include "jstd/basic/inttypes.h"

#include <stdio.h>

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>

namespace jtest {

//
// An individual benchmark result.
//
struct Result {
    typedef std::size_t size_type;

    size_type   catId;
    std::string name;
    double      elaspedTime1;
    size_type   checksum1;
    double      elaspedTime2;
    size_type   checksum2;

    Result() noexcept
        : catId(size_type(-1)),
          elaspedTime1(0.0), checksum1(0),
          elaspedTime2(0.0), checksum2(0) {
    }

    Result(size_type catId, const std::string & name,
           double elaspedTime1, size_type checksum1,
           double elaspedTime2, size_type checksum2)
        : catId(catId), name(name),
          elaspedTime1(elaspedTime1), checksum1(checksum1),
          elaspedTime2(elaspedTime2), checksum2(checksum2) {
    }

    Result(const Result & src)
        : catId(src.catId), name(src.name),
          elaspedTime1(src.elaspedTime1), checksum1(src.checksum1),
          elaspedTime2(src.elaspedTime2), checksum2(src.checksum2) {
    }

    Result(Result && src)
        : catId(size_type(-1)),
          elaspedTime1(0.0), checksum1(0),
          elaspedTime2(0.0), checksum2(0) {
        if (&src != this) {
            this->catId         = src.catId;
            this->name          = std::move(src.name);
            this->elaspedTime1  = src.elaspedTime1;
            this->checksum1     = src.checksum1;
            this->elaspedTime2  = src.elaspedTime2;
            this->checksum2     = src.checksum2;
        }
    }

    void swap(Result & rhs) noexcept {
        if (&rhs != this) {
            std::swap(this->catId,          rhs.catId);
            std::swap(this->name,           rhs.name);
            std::swap(this->elaspedTime1,   rhs.elaspedTime1);
            std::swap(this->checksum1,      rhs.checksum1);
            std::swap(this->elaspedTime2,   rhs.elaspedTime2);
            std::swap(this->checksum2,      rhs.checksum2);
        }
    }
};

class BenchmarkCategory {
public:
    typedef std::size_t size_type;

private:
    size_type           cat_id_;
    std::string         name_;
    std::vector<Result> result_list_;

public:
    BenchmarkCategory(const std::string & name) : cat_id_(size_type(-1)), name_(name) {}
    ~BenchmarkCategory() {}

    size_type size() const { return result_list_.size(); }

    size_type getCatId() const {
        return this->cat_id_;
    }

    std::string & name() {
        return this->name_;
    }

    const std::string & name() const {
        return this->name_;
    }

    void setName(const std::string & name) {
        this->name_ = name;
    }

    Result & getResult(size_type index) {
        return result_list_[index];
    }

    const Result & getResult(size_type index) const {
        return result_list_[index];
    }

    void addResult(size_type cat_id, const std::string & name,
                   double time1, size_type checksum1,
                   double time2, size_type checksum2) {
        this->cat_id_ = cat_id;
        Result result(cat_id, name, time1, checksum1, time2, checksum2);
        result_list_.push_back(std::move(result));
    }
};

class BenchmarkResult {
public:
    typedef std::size_t size_type;

private:
    std::string name1_;
    std::string name2_;
    std::vector<BenchmarkCategory *> category_list_;

    void destroy() {
        for (size_type i = 0; i < category_list_.size(); i++) {
            BenchmarkCategory * category = category_list_[i];
            if (category != nullptr) {
                delete category;
                category_list_[i] = nullptr;
            }
        }
        category_list_.clear();
    }

public:
    BenchmarkResult() {}
    ~BenchmarkResult() {
        destroy();
    }

    size_type category_size() const {
        return category_list_.size();
    }

    std::string & getName1() {
        return this->name1_;
    }

    std::string & getName2() {
        return this->name2_;
    }

    const std::string & getName1() const {
        return this->name1_;
    }

    const std::string & getName2() const {
        return this->name2_;
    }

    void setName(const std::string & name1, const std::string & name2) {
        this->name1_ = name1;
        this->name2_ = name2;
    }

    BenchmarkCategory * getCategory(size_type index) {
        if (index < category_list_.size())
            return category_list_[index];
        else
            return nullptr;
    }

    size_type addCategory(const std::string & name) {
        BenchmarkCategory * category = new BenchmarkCategory(name);
        category_list_.push_back(category);
        return (category_list_.size() - 1);
    }

    bool addResult(size_type catId, const std::string & name,
                   double elaspedTime1, size_type checksum1,
                   double elaspedTime2, size_type checksum2) {
        BenchmarkCategory * category = getCategory(catId);
        if (category != nullptr) {
            category->addResult(catId, name, elaspedTime1, checksum1, elaspedTime2, checksum2);
            return true;
        }
        return false;
    }

    bool addBlankLine(size_type catId) {
        BenchmarkCategory * category = getCategory(catId);
        if (category != nullptr) {
            category->addResult(catId, "_blank", 0.0, 0, 0.0, 0);
            return true;
        }
        return false;
    }

    std::string formatMsTime(double fMillisec) {
        char time_buf[256];

        if (fMillisec >= 1000.0 * 1000.0) {
            snprintf(time_buf, sizeof(time_buf), "%7.2f Min", fMillisec / (60 * 1000.0));
        }
        else if (fMillisec >= 1000.0 * 10.0) {
            snprintf(time_buf, sizeof(time_buf), "%7.2f Sec", fMillisec / 1000.0);
        }
        else if (fMillisec >= 1.0 * 0.1) {
            snprintf(time_buf, sizeof(time_buf), "%7.2f ms ", fMillisec);
        }
        else if (fMillisec >= 0.001 * 0.1) {
            snprintf(time_buf, sizeof(time_buf), "%7.2f us ", fMillisec * 1000.0);
        }
        else {
            snprintf(time_buf, sizeof(time_buf), "%7.2f ns ", fMillisec * 1000000.0);
        }

        return std::string(time_buf);
    }

    /*******************************************************************************************************
       Test                                          std::unordered_map         jstd::Dictionary     Ratio
      ------------------------------------------------------------------------------------------------------
       hash_map<std::string, std::string>          checksum     time         checksum    time

       hash_map<K, V>/find                    | 98765432109   100.00 ms | 98765432109   30.00 ms |   3.33
       hash_map<K, V>/insert                  | 98765432109   100.00 ms | 98765432109   30.00 ms |   3.33
       hash_map<K, V>/emplace                 | 98765432109   100.00 ms | 98765432109   30.00 ms |   3.33
       hash_map<K, V>/erase                   | 98765432109   100.00 ms | 98765432109   30.00 ms |   3.33
      ------------------------------------------------------------------------------------------------------
    *******************************************************************************************************/
    void printResult(const std::string & filename, double totalElapsedTime = 0.0) {
        for (size_type catId = 0; catId < category_size(); catId++) {
            BenchmarkCategory * category = getCategory(catId);
            if (category != nullptr) {
                printf(" Test                                    %23s   %23s      Ratio\n",
                       this->name1_.c_str(), this->name2_.c_str());
                printf("--------------------------------------------------------------------------------------------------------\n");
                printf("\n");
                if (category->name().size() <= 40)
                    printf(" %-40s    checksum    time          checksum    time\n", category->name().c_str());
                else
                    printf(" %-52s"          "    time          checksum    time\n", category->name().c_str());
                printf("\n");

                size_type result_count = category->size();
                for (size_type i = 0; i < result_count; i++) {
                    const Result & result = category->getResult(i);
                    double ratio;
                    if (result.elaspedTime2 != 0.0)
                        ratio = result.elaspedTime1 / result.elaspedTime2;
                    else
                        ratio = 0.0;
                    if (result.name != "_blank") {
                        printf(" %-38s | %11" PRIuPTR " %11s | %11" PRIuPTR " %11s |   %0.2f\n",
                               result.name.c_str(),
                               result.checksum1, formatMsTime(result.elaspedTime1).c_str(),
                               result.checksum2, formatMsTime(result.elaspedTime2).c_str(),
                               ratio);
                    }
                    else {
                        printf("\n");
                    }
                }

                if (catId < (category_size() - 1))
                    printf("\n\n");
            }
        }

        printf("\n");
        printf("--------------------------------------------------------------------------------------------------------\n");
        printf("\n");
        if (filename.size() == 0 || filename.c_str() == nullptr || filename == "")
            printf("Dict filename: %-52s  Total elapsed time: %0.2f ms\n", "header_fields[]", totalElapsedTime);
        else
            printf("Dict filename: %-52s  Total elapsed time: %0.2f ms\n", filename.c_str(), totalElapsedTime);
        printf("\n");
    }
};

} // namespace jtest

#endif // JTEST_BENCHMARK_H
