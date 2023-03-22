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


//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
hash_table<K, V, Hash, Index, Equal, RehashingFactor>::entry::entry(uint256 hash_code, K key, V value)
        : hash_code(std::move(hash_code)), key(key) {
    values.push_front(value);
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
hash_table<K, V, Hash, Index, Equal, RehashingFactor>::entry::~entry() = default;

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
float hash_table<K, V, Hash, Index, Equal, RehashingFactor>::load_factor() {
    return key_count() * 1.0 / bucket_count();
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
void hash_table<K, V, Hash, Index, Equal, RehashingFactor>::rehashing() {
    auto *new_buckets = new std::list<entry>[b * RehashingFactor];
    int j{};

    for (int i = 0; i < b; ++i) {
        for (entry &e: buckets[i]) {
            j = (int) (e.hash_code % (b * RehashingFactor));
            new_buckets[j].push_back(e);
        }
        buckets[i].clear();
    }

    delete[] buckets;
    buckets = new_buckets;
    b *= RehashingFactor;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
hash_table<K, V, Hash, Index, Equal, RehashingFactor>::hash_table(Index index, Hash hash, Equal equal, int b, float max_load_factor)
        : index(index),
          b(b),
          v(0),
          k(0),
          hash{hash},
          equal{equal},
          max_load_factor(max_load_factor),
          buckets(nullptr) {
    buckets = new std::list<entry>[b];
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
hash_table<K, V, Hash, Index, Equal, RehashingFactor>::~hash_table() {
    this->clear();
    delete[] buckets;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
int hash_table<K, V, Hash, Index, Equal, RehashingFactor>::bucket_count() {
    return this->b;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>int
hash_table<K, V, Hash, Index, Equal, RehashingFactor>::key_count() {
    return this->k;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
int hash_table<K, V, Hash, Index, Equal, RehashingFactor>::bucket_size(int i) {
    if (i >= b) {
        throw std::runtime_error("\"i\" value exceeds hash table size");
    }
    return buckets[i].size();
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>int hash_table<K, V, Hash, Index, Equal, RehashingFactor>::size() {
    return this->v;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>bool hash_table<K, V, Hash, Index, Equal, RehashingFactor>::empty() {
    return key_count() == 0;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
void hash_table<K, V, Hash, Index, Equal, RehashingFactor>::clear() {
    if (empty()) {
        return;
    }

    for (int i = 0; i < b; ++i) {
        buckets[i].clear();
    }
    k = 0;
    v = 0;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
bool hash_table<K, V, Hash, Index, Equal, RehashingFactor>::find(K key) {
    uint256 hash_code = hash(key);
    int i = (int) (hash_code % b);
    std::list<entry> &bucket = buckets[i];

    for (entry &e: bucket) {
        if (equal(e.key, key)) {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
void hash_table<K, V, Hash, Index, Equal, RehashingFactor>::insert(V value) {
    K key = index(value);
    uint256 hash_code = hash(key);
    int i = (int) (hash_code % b);
    std::list<entry> &bucket = buckets[i];

    EntryIterator it = std::find_if(bucket.begin(), bucket.end(), [&](entry &e) { return equal(e.key, key); });
    if (it != bucket.end()) {
        it->values.push_front(value);
    } else {
        bucket.push_front(entry(hash_code, key, value));
        ++k;
    }

    ++v;
    if (load_factor() >= max_load_factor) {
        rehashing();
    }
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
bool hash_table<K, V, Hash, Index, Equal, RehashingFactor>::remove(K key) {
    uint256 hash_code = hash(key);
    int i = (int) (hash_code % b);
    std::list<entry> &bucket = buckets[i];

    EntryIterator it = std::find_if(bucket.begin(), bucket.end(), [&](entry &e) { return equal(e.key, key); });
    if (it != bucket.end()) {
        v -= it->values.size();
        --k;
        bucket.erase(it);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>std::list<V>
hash_table<K, V, Hash, Index, Equal, RehashingFactor>::search(K key) {
    uint256 hash_code = hash(key);
    int i = (int) (hash_code % b);
    std::list<entry> &bucket = buckets[i];

    for (entry &e: bucket) {
        if (equal(e.key, key)) {
            return e.values;
        }
    }
    return std::list<V>{};
}

//-----------------------------------------------------------------------------

template<typename K, typename V, typename Hash, typename Index, typename Equal, uint8 RehashingFactor>
requires Constraint<RehashingFactor>
void hash_table<K, V, Hash, Index, Equal, RehashingFactor>::print(std::ostream &os, Print<V> print_value, Print<K> print_key) {
    for (int i = 0; i < bucket_count(); ++i) {
        os << "[" << i << "]";
        for (const entry &e: buckets[i]) {
            os << " ==> [";
            print_key(os, e.key);
            os << ":(";
            for (auto it = e.values.begin(); it != e.values.end(); ++it) {
                print_value(os, *it);
                os << ((std::next(it, 1) != e.values.end()) ? "," : "");
            }
            os << ")]";
        }
        os << std::endl;
    }
}


#endif //HASH_TABLE_HASHTABLE_HPP
