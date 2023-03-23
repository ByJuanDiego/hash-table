//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHTABLE_HPP
#define HASH_TABLE_HASHTABLE_HPP

#include <functional>
#include <algorithm>
#include <stdexcept>
#include <list>
#include <utility>

#include <boost/multiprecision/cpp_int.hpp>


using uint8 = boost::uint8_t;
using uint256 = boost::multiprecision::uint256_t;

#ifndef MINIMUM_REHASHING_FACTOR
#define MINIMUM_REHASHING_FACTOR 2
#endif

#ifndef MAXIMUM_REHASHING_FACTOR
#define MAXIMUM_REHASHING_FACTOR 5
#endif

#ifndef DEFAULT_N_BUCKETS
#define DEFAULT_N_BUCKETS 11
#endif

#ifndef DEFAULT_MAX_LOAD_FACTOR
#define DEFAULT_MAX_LOAD_FACTOR 0.75f
#endif

template<typename Printable>
using Print = std::function<void(std::ostream &, Printable)>;

template<typename Printable>
Print<Printable> print = [](std::ostream &os, const Printable &p) { os << p; };

template<uint8 N>
concept Constraint = (N >= MINIMUM_REHASHING_FACTOR && N <= MAXIMUM_REHASHING_FACTOR);

template<
        typename K,
        typename V,
        typename Hash = std::hash<K>,
        typename Index = std::function<K(V)>,
        typename Equal = std::equal_to<K>,
        uint8 RehashingFactor = MINIMUM_REHASHING_FACTOR
> requires Constraint<RehashingFactor>
class hash_table {

public:

    struct entry {
        K key;
        uint256 hash_code;
        std::list<V> values;

        explicit entry(uint256 hash_code, K key, V value);

        ~entry();
    };

    typedef std::list<entry>::iterator EntryIterator;

private:

    int k;
    int v;
    int b;
    Hash hash;
    Index index;
    Equal equal;
    float max_load_factor;
    std::list<entry> *buckets;

    float load_factor();

    void rehashing();

public:

    explicit hash_table(Index index,
                        Hash hash = Hash(),
                        Equal equal = Equal(),
                        int b = DEFAULT_N_BUCKETS,
                        float max_load_factor = DEFAULT_MAX_LOAD_FACTOR);

    ~hash_table();

    int bucket_count();

    int key_count();

    int bucket_size(int i);

    int size();

    bool empty();

    void clear();

    bool find(K key);

    void insert(V value);

    bool remove(K key);

    std::list<V> search(K key);

    void print(std::ostream &os, Print<V> print_value = ::print<V>, Print<K> print_key = ::print<K>);

};


#endif //HASH_TABLE_HASHTABLE_HPP
