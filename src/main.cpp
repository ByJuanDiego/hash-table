#include <iostream>
#include <fstream>

#include "transaction.hpp"
#include "hashfunction.hpp"
#include "hashtable.cpp"

int main() {
    sha2::sha256<std::string> hash;
    std::function<std::string(transaction *)> index = [&](const transaction *tx) -> std::string {
        return tx->emisor;
    };
    std::function<bool(std::string, std::string)> equal = [&](const std::string& a, const std::string& b) -> bool {
        return (a == b);
    };
    hash_table<std::string, transaction *, decltype(hash), decltype(index), decltype(equal)> hashTable(index, hash, equal);

    std::ifstream file("./utils/assets/transactions.txt");
    std::string emisor, receptor;
    int amount;
    std::list<transaction *> destructor;

    while (file >> emisor >> receptor >> amount) {
        auto *tx = new transaction(emisor, receptor, amount);
        hashTable.insert(tx);
        destructor.push_back(tx);
    }

    hashTable.print(std::cout, [&](std::ostream &os, const transaction *tx) { os << *tx; });
    std::cout << std::endl;

    std::string key = "juan-diego";
    if (hashTable.find(key)) {
        std::cout << "key found:" << std::endl;
        for (const transaction *t: hashTable.search(key)) {
            std::cout << t->to_string() << std::endl;
        }
        std::cout << std::endl;
    }

    if (hashTable.remove(key)) {
        std::cout << "key removed:" << std::endl;
        hashTable.print(std::cout, [&](std::ostream &os, const transaction *tx) { os << *tx; });
    }

    hashTable.clear();
    for (transaction *tx: destructor) {
        delete tx;
    }
    return 0;
}
