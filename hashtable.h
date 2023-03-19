//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHTABLE_H
#define HASH_TABLE_HASHTABLE_H

#include <functional>
#include <algorithm>
#include <stdexcept>
#include <list>

#define REHASING_FACTOR 2

template<typename K, typename V, typename Hash = std::hash<K>, class KeyEqual = std::equal_to<K>>
class hash_table {
private:

    struct node {
        size_t hash_code;
        K key;
        V value;

        explicit node(size_t hash_code, K key, V value) : hash_code(hash_code), key(key), value(value) {}
    };

    Hash hash;
    KeyEqual equal;
    int n_buckets;
    int n_elements;
    float max_load_factor;
    std::list<node> *buckets;

    float load_factor() {
        return size() * 1.0 / bucket_count();
    }

    void rehashing() {
        auto *new_buckets = new std::list<node>[n_buckets * REHASING_FACTOR];
        int j{};

        for (int i = 0; i < n_buckets; ++i) {
            for (node &e: buckets[i]) {
                j = e.hash_code % (n_buckets * REHASING_FACTOR);
                new_buckets[j].push_front(e);
            }
            buckets[i].clear();
        }

        delete[] buckets;
        buckets = new_buckets;
        n_buckets *= REHASING_FACTOR;
    }

public:

    explicit hash_table(int n_buckets = 5, float max_load_factor = 0.5, Hash hash = Hash(), KeyEqual equal = KeyEqual())
            : n_buckets(n_buckets),
              n_elements(0),
              hash{hash},
              equal{equal},
              max_load_factor(max_load_factor),
              buckets(nullptr) {
        buckets = new std::list<node>[n_buckets];
    }

    ~hash_table() {
        this->clear();
        delete[] buckets;
    }

    bool find(K key) {
        try {
            get(key);
        }
        catch (const std::exception &e) {
            return false;
        }
        return true;
    }

    bool set(K key, V value) {
        size_t hash_code = hash(key);
        int i = (hash_code % n_buckets);
        std::list<node> &bucket = buckets[i];

        auto it = std::find_if(bucket.begin(), bucket.end(), [&](node &e) { return equal(e.key, key); });
        if (it != bucket.end()) {
            return false;
        }

        bucket.push_front(node(hash_code, key, value));
        ++n_elements;
        if (load_factor() >= max_load_factor) {
            rehashing();
        }
        return true;
    }

    V &get(K key) {
        unsigned long hash_code = hash(key);
        int i = hash_code % n_buckets;
        std::list<node> &bucket = buckets[i];

        for (node &e: bucket) {
            if (!equal(e.key, key)) {
                continue;
            }
            return e.value;
        }

        throw std::runtime_error("The key you are looking for do not exists");
    }

    bool remove(K key) {
        size_t hash_code = hash(key);
        int i = hash_code % n_buckets;

        if (buckets[i].remove_if([&](node &e) { return equal(e.key, key); }) == 1) {
            --n_elements;
            return true;
        }

        return false;
    }

    bool update(K key, V new_value) {
        unsigned long hash_code = hash(key);
        int i = hash_code % n_buckets;
        std::list<node> &bucket = buckets[i];

        for (node &e: bucket) {
            if (!equal(e.key, key)) {
                continue;
            }
            e.value = new_value;
            return true;
        }
        return false;
    }

    void clear() {
        for (int i = 0; i < n_buckets; ++i)
            buckets[i].clear();
        n_elements = 0;
    }

    int size() {
        return this->n_elements;
    }

    int bucket_count() {
        return this->n_buckets;
    }

    int bucket_size(int i) {
        return buckets[i].size();
    }

    void print(std::ostream &os) {
        for (int i = 0; i < bucket_count(); ++i) {
            os << "[" << i << "] => ";
            for (node &e: buckets[i]) {
                os << "{" << e.key << ":" << e.value << "}" << " -> ";
            }
            os << std::endl;
        }
    }
};

#endif //HASH_TABLE_HASHTABLE_H
