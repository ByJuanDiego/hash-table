#include <iostream>
#include <functional>
#include <iomanip>
#include <openssl/sha.h>
#include "hashtable.h"

std::string sha256(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (unsigned char i: hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) i;
    }
    return ss.str();
}

int main() {
    std::hash<std::string> h;
    typedef std::function<int(std::string)> my_hash;
    my_hash function = [&](const std::string &s) { return h(sha256(s)); };
    hash_table<std::string, int, my_hash> hashTable(5, function, 0.6);

    hashTable.set("key1", 13);
    hashTable.set("key2", 10);
    hashTable.set("key3", 15);

    hashTable.print(std::cout);
    std::cout << std::endl;

    if (hashTable.find("key1")) {
        std::cout << "key1: " << hashTable.get("key1") << std::endl << std::endl;
    }

    if (hashTable.remove("key2")) {
        hashTable.print(std::cout);
        std::cout << std::endl;
    }

    if (!hashTable.find("key2")) {
        std::cout << "number of elements: " << hashTable.size() << std::endl;
        std::cout << "number of buckets: " << hashTable.bucket_count() << std::endl;
        std::cout << "number of buckets: " << hashTable.bucket_size(8) << std::endl;
        std::cout << std::endl;
    }

    if (!hashTable.update("key2", 0) && hashTable.update("key1", 0)) {
        hashTable.set("key4", 5);
        hashTable.print(std::cout);
        std::cout << std::endl;
    }

    hashTable.clear();
    hashTable.print(std::cout);

    return 0;
}
