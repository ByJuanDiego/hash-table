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

template<typename KT, typename KV, typename F = std::hash<KT>>
class hash_table {
private:

    struct entry {
        unsigned long hash_code;
        KT key;
        KV value;

        explicit entry(unsigned long hash_code, KT key, KV value) : hash_code(hash_code), key(key), value(value) {}
    };

    F hash;
    int n_buckets;
    int n_elements;
    float max_load_factor;
    std::list<entry> *buckets;

    float load_factor() {
        return size() * 1.0 / bucket_count();
    }

    void rehashing() {
        auto *new_buckets = new std::list<entry>[n_buckets * REHASING_FACTOR];
        int j{};

        for (int i = 0; i < n_buckets; ++i) {
            for (entry &e: buckets[i]) {
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

    explicit hash_table(int n_buckets = 5, F hash = F(), float max_load_factor = 0.5)
            : n_buckets(n_buckets),
              n_elements(0),
              hash{hash},
              max_load_factor(max_load_factor),
              buckets(nullptr) {
        buckets = new std::list<entry>[n_buckets];
    }

    ~hash_table() {
        this->clear();
        delete[] buckets;
    }

    bool find(KT key) {
        try {
            get(key);
        }
        catch (const std::exception &e) {
            return false;
        }
        return true;
    }

    bool set(KT key, KV value) {
        unsigned long hash_code = hash(key);
        int i = hash_code % n_buckets;
        std::list<entry> &bucket = buckets[i];

        auto it = std::find_if(bucket.begin(), bucket.end(), [&](entry &e) { return e.key == key; });
        if (it != bucket.end()) {
            return false;
        }

        bucket.push_front(entry(hash_code, key, value));
        ++n_elements;
        if (load_factor() >= max_load_factor) {
            rehashing();
        }
        return true;
    }

    KV &get(KT key) {
        unsigned long hash_code = hash(key);
        int i = hash_code % n_buckets;
        std::list<entry> &bucket = buckets[i];

        for (entry &e: bucket) {
            if (e.key != key) {
                continue;
            }
            return e.value;
        }

        throw std::runtime_error("The key you are looking for do not exists");
    }

    bool remove(KT key) {
        unsigned long hash_code = hash(key);
        int i = hash_code % n_buckets;

        if (buckets[i].remove_if([&](entry &e) { return e.key == key; }) == 1) {
            --n_elements;
            return true;
        }

        return false;
    }

    bool update(KT key, KV new_value) {
        unsigned long hash_code = hash(key);
        int i = hash_code % n_buckets;
        std::list<entry> &bucket = buckets[i];

        for (entry &e: bucket) {
            if (e.key != key) {
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
            for (entry &e: buckets[i]) {
                os << "{" << e.key << ":" << e.value << "}" << " -> ";
            }
            os << std::endl;
        }
    }
};

#endif //HASH_TABLE_HASHTABLE_H
