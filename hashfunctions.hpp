//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHFUNCTIONS_HPP
#define HASH_TABLE_HASHFUNCTIONS_HPP

#include <functional>
#include <iomanip>
#include <string>

#include <openssl/sha.h>

#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>

#ifndef UINT256
#define UINT256

#include <boost/multiprecision/cpp_int.hpp>

using uint256 = boost::multiprecision::uint256_t;
#endif // UINT256

namespace sha2 {
    boost::cnv::cstream converter;

    uint256 to_uint256(const std::string &hex) {
        converter(std::hex)(std::skipws);
        auto f = apply<uint256>(std::ref(converter)).value_or(-1);
        return f(hex);
    }

    std::string get_sha256(const std::string &str) {
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

    template<typename K>
    struct sha256 {
        uint256 operator()(const K &key) {
            return sha2::to_uint256(sha2::get_sha256(std::to_string(key)));
        }
    };

    template<>
    struct sha256<std::string> {
        uint256 operator()(const std::string &key) {
            return sha2::to_uint256(sha2::get_sha256(key));
        }
    };
}

#endif //HASH_TABLE_HASHFUNCTIONS_HPP