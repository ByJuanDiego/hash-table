//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHTABLE_HPP
#define HASH_TABLE_HASHTABLE_HPP

#include <functional>
#include <stdexcept>
#include <list>

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

/// Template callback to display types that overloads operator<< and built-in types
template<typename Printable>
Print<Printable> print = [](std::ostream &os, const Printable &p) { os << p; };

/// Constraint to limit the values of RehashingFactor before runtime
template<uint8 N>
concept Constraint = (N >= MINIMUM_REHASHING_FACTOR && N <= MAXIMUM_REHASHING_FACTOR);

/**
 * Hash index for queries.
 *
 *
 * @tparam K the type of the key
 * @tparam V the type of data stored in the hash table
 * @tparam Hash callable used to obtain a hash code from a determined key
 * @tparam Index callable used to obtain the index of a value
 * @tparam Equal callable used to define when two keys are equal
 * @tparam RehashingFactor 8-bits integer that defines the table size multiplier when rehashing ocurrs
 */
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

        K key;                      ///< Key of the entry
        uint256 hash_code;          ///< Hash code of the key
        std::list<V> values;        ///< List of values with same key

        /// Constructs an entry
        explicit entry(uint256 hash_code, K key, V value);

        /// Destructs an entry
        ~entry();

    };

    typedef std::list<entry>::iterator EntryIterator;

private:

    int k;                         ///< Total number of keys
    int v;                         ///< Total number of values
    int b;                         ///< Total number of buckets
    Hash hash;                     ///< Receives a key and generates his hash code
    Index index;                   ///< Receives a value and returns his index attribute
    Equal equal;                   ///< Receives two keys and returns true if both are equal and false otherwise
    float max_load_factor;         ///< Fill threshold; when exeeded, rehashing occurs
    std::list<entry> *buckets;     ///< Pointer to the array where entry lists are located

    ///< Returns the number of keys divided by the number of buckets
    float load_factor();

    ///< Creates a new hash table and re-calculate the buckets for each entry in order to maintain the searching performance
    void rehashing();

public:

    /// Constructs a hash table
    explicit hash_table(Index index,
                        Hash hash = Hash(),
                        Equal equal = Equal(),
                        int b = DEFAULT_N_BUCKETS,
                        float max_load_factor = DEFAULT_MAX_LOAD_FACTOR);

    /// Destructs a hash table
    ~hash_table();

    /// Returns `b`
    int bucket_count();

    /// Returns `k`
    int key_count();

    /**
     * Returns the number of entries in a bucket
     *
     * @param i the index of the array to be accessed
     * @throws std::rutime_error if `i` is greater or equal to `b`
     */
    int bucket_size(int i);

    /// Returns `v`
    int size();

    /// Returns `true` if the hash table is empty and `false` otherwhise
    bool empty();

    /// Frees up the memory and allows to continue using the hashtable
    void clear();

    /**
     * Returns if a key exists in the hash table
     *
     * @param key the key to find
     * @return `true` if the key exists and `false` otherwise
     */
    bool find(K key);

    /**
     * Inserts a new value in the hash table and creates an entry if the key not exists.
     * Occassionaly, when a certain number of entries are created, rehashing occurs
     *
     * @param value the value to be inserted
     */
    void insert(V value);

    /**
     * Removes the entry that contains all the values with a determined key
     *
     * @param key the key to be deleted
     * @return `true` if the entry was found and `false` otherwise
     */
    bool remove(K key);

    /**
     * Search all the values associated to a key
     *
     *
     * @param key the key to be searched
     * @return a list with all the values whose keys are equal to `key`. Returns empty list if the `key` not exists.
     */
    std::list<V> search(K key);

    /**
     * Print all the hash table
     *
     * @param os std::ostream object to output stream the hash table in console or load it to a file
     * @param print_value function parameter that defines how to print a `V` type
     * @param print_key function parameter that defines how to print a `K` type
     */
    void print(std::ostream &os, Print<V> print_value = ::print<V>, Print<K> print_key = ::print<K>);

};


#endif //HASH_TABLE_HASHTABLE_HPP
