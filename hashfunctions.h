//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHFUNCTIONS_H
#define HASH_TABLE_HASHFUNCTIONS_H

#include <openssl/sha.h>
#include <functional>
#include <iomanip>
#include <string>

#define SHA256_SIZE 10

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
    return ss.str().substr(0, SHA256_SIZE);
}

template<typename KT>
struct hash_sha256 {
    size_t operator()(const KT &key) {
        std::string hex_hash = sha256(key);
        return std::stoull(hex_hash, nullptr, 16);
    }
};


#endif //HASH_TABLE_HASHFUNCTIONS_H
