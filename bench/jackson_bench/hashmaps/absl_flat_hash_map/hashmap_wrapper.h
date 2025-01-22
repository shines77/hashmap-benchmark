// /jackson_ben/hashmaps/absl_flat_hash_map/hashmap_wrapper.h
// Copyright (c) 2024 Jackson L. Allan.
// Distributed under the MIT License (see the accompanying LICENSE file).

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpedantic"
//#pragma GCC diagnostic ignored "-Woverflow"
//#include <absl/container/internal/raw_hash_set.cc>
//#include <absl/base/internal/raw_logging.cc>
//#include <absl/hash/internal/hash.cc>
//#include <absl/hash/internal/city.cc>
//#include <absl/hash/internal/low_level_hash.cc>
#include <absl/container/flat_hash_map.h>
//#pragma GCC diagnostic pop

template <typename BluePrint>
struct absl_flat_hash_map
{
    using key_type = typename BluePrint::key_type;
    using value_type = typename BluePrint::value_type;

    struct hash {
        using is_avalanching = void;
        using argument_type = key_type;
        using result_type = std::size_t;

        std::size_t operator () (const key_type & key) const {
            return BluePrint::hash_key(key);
        }
    };

    struct cmpr {
        bool operator () (const key_type & key_1, const key_type & key_2) const {
            return BluePrint::cmpr_keys(key_1, key_2);
        }
    };

    using table_type = absl::flat_hash_map<
        key_type,
        value_type,
        hash,
        cmpr
    >;

    using iterator = typename table_type::iterator;
    using const_iterator = typename table_type::const_iterator;

    static table_type & create_table()
    {
        static table_type table;
        table.max_load_factor(MAX_LOAD_FACTOR);
        return table;
    }

    static iterator find(table_type & table, const key_type & key)
    {
        return table.find(key);
    }

    static void insert(table_type & table, const key_type & key)
    {
        table[key] = value_type();
    }

    static void erase(table_type & table, const key_type & key)
    {
        table.erase(key);
    }

    static iterator begin_iter(table_type & table)
    {
        return table.begin();
    }

    static bool is_iter_valid(table_type & table, iterator & iter)
    {
        return (iter != table.end());
    }

    static void increment_iter(table_type & table, iterator & iter)
    {
        ++iter;
    }

    static const key_type & get_key_from_iter(table_type & table, iterator & iter)
    {
        return iter->first;
    }

    static const value_type & get_value_from_iter(table_type & table, iterator & iter)
    {
        return iter->second;
    }

    static void destroy_table(table_type & table)
    {
        // RAII handles destruction.
    }
};

template <>
struct absl_flat_hash_map<void>
{
    static constexpr const char * name = "absl::flat_hash_map";
    static constexpr const char * label = "absl";
    static constexpr const char * color = "rgb( 81, 169, 240 )";
    static constexpr bool tombstone_like_mechanism = true;
};
