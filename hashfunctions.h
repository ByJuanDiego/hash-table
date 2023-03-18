//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHFUNCTIONS_H
#define HASH_TABLE_HASHFUNCTIONS_H

#include <openssl/sha.h>
#include <functional>
#include <iomanip>
#include <string>

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

template<typename KT>
struct hash_sha256 {
    unsigned long operator()(const KT &key) {
        std::hash<KT> h;
        return h(sha256(key));
    }
};


#endif //HASH_TABLE_HASHFUNCTIONS_H
