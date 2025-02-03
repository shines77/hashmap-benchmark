// /bench/jackson_bench/hashmaps/boost_unordered_flat_map/hashmap_wrapper.h
// Copyright (c) 2024 Jackson L. Allan.
// Distributed under the MIT License (see the accompanying LICENSE file).

#include <boost/unordered/unordered_flat_map.hpp>

template <typename BluePrint>
struct boost_unordered_flat_map
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

    using table_type = boost::unordered::unordered_flat_map<
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
        //table[key] = value_type();
        table.emplace(key, value_type());
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
struct boost_unordered_flat_map<void>
{
    static constexpr const char * name = "boost::unordered_flat_map";
    static constexpr const char * label = "boost";
    static constexpr const char * color = "rgb( 104, 110, 230 )";
    static constexpr bool tombstone_like_mechanism = true;
};
