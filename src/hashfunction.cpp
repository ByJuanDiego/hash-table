//
// Created by juandiego on 3/22/23.
//

#include "hashfunction.hpp"

//-----------------------------------------------------------------------------

std::string sha2::get_sha256(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (unsigned char i: hash) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned) i;
    }
    return ss.str();
}

//-----------------------------------------------------------------------------

uint256 sha2::to_uint256(const std::string &hex) {
    boost::cnv::cstream converter;
    converter(std::hex)(std::skipws);
    auto f = apply<uint256>(std::ref(converter));
    return f(hex);
}

//-----------------------------------------------------------------------------

template<typename K>
uint256 sha2::sha256<K>::operator()(const K &key) {
    return sha2::to_uint256(sha2::get_sha256(std::to_string(key)));
}

//-----------------------------------------------------------------------------

uint256 sha2::sha256<std::string>::operator()(const std::string &key) {
    return sha2::to_uint256(sha2::get_sha256(key));
}

//-----------------------------------------------------------------------------
