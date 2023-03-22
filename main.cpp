#include <iostream>
#include <fstream>

#include "hashfunctions.hpp"
#include "hashtable.cpp"

struct transaction {
    std::string emisor;
    std::string receptor;
    int amount;

    explicit transaction(std::string emisor, std::string receptor, int amount) :
            emisor(std::move(emisor)), receptor(std::move(receptor)), amount(amount) {}

    ~transaction() = default;

    [[nodiscard]] std::string to_string() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const transaction &transaction) {
        os << "{" << transaction.emisor << "," << transaction.receptor << "," << transaction.amount << "}";
        return os;
    }
};

int main() {
    hash_table<int, transaction *, sha2::sha256<int>, std::function<int(transaction *)>> hashTable(
            [&](const transaction *tx) -> int {
                return tx->amount;
            });

    std::ifstream file("transactions.txt");
    std::string emisor, receptor;
    int amount;
    std::list<transaction *> txs_destructor;

    while (file >> emisor >> receptor >> amount) {
        auto *tx = new transaction(emisor, receptor, amount);
        hashTable.insert(tx);
        txs_destructor.push_back(tx);
    }

    hashTable.print(std::cout, [&](std::ostream &os, const transaction *tx) { os << *tx; });
    std::cout << std::endl;

    int key = 70311;
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
    for (transaction *tx: txs_destructor) {
        delete tx;
    }
    return 0;
}
