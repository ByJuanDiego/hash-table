#include <iostream>
#include "hashtable.h"
#include "hashfunctions.h"

struct transaction {
    std::string emisor;
    std::string receptor;
    double amount;

    explicit transaction(std::string emisor, std::string receptor, double amount) :
            emisor(std::move(emisor)), receptor(std::move(receptor)), amount(amount) {}

    [[nodiscard]] std::string to_string() const {
        std::stringstream ss;
        ss << emisor << receptor << amount;
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, transaction &transaction) {
        os << "(" << transaction.emisor << "," << transaction.receptor << "," << transaction.amount << ")";
        return os;
    }
};

template<>
struct hash_sha256<transaction> {
    size_t operator()(const transaction &transaction) {
        hash_sha256<std::string> h;
        return h(transaction.to_string());
    }
};

template<>
struct std::hash<transaction> {
    size_t operator()(const transaction &transaction) {
        std::hash<std::string> h;
        return h(transaction.to_string());
    }
};

template<>
struct std::equal_to<transaction> {
    bool operator()(const transaction &u, const transaction &v) {
        std::equal_to<> equal;
        return equal(u.amount, v.amount) && equal(u.emisor, v.emisor) && equal(u.receptor, v.receptor);
    }
};

int main() {
    hash_table<transaction, std::string, hash_sha256<transaction>> hashTable(5, 0.5);

    hashTable.set(transaction("samanta", "anderson", 2.4), "guarana");
    hashTable.set(transaction("juan diego", "samanta", 24), "vegan hot dog");

    hashTable.print(std::cout);
    std::cout << std::endl;

    transaction tx{"juan diego", "samanta", 24};
    if (hashTable.find(tx)) {
        std::cout << hashTable.get(tx) << std::endl << std::endl;
    }

    hashTable.update(tx, "super vegan hot dog");
    hashTable.print(std::cout);
    std::cout << std::endl;

    hashTable.remove(tx);
    hashTable.print(std::cout);

    return 0;
}
