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
#include <boost/multiprecision/cpp_int.hpp>


using uint256 = boost::multiprecision::uint256_t;

/**
 * @namespace to group functions and structures related to hashing
 */
namespace sha2
{
    /**
     * transform a 64 length hexadecimal to a 256 bit unsigned integer
     *
     *
     * @param hex contains the hexadecimal
     */
    uint256 to_uint256(const std::string &hex);

    /**
     * hashes a string of any length and gets his correspondant sha256 value
     *
     *
     * @param str contains the string
     * @return sha256(str) in hexadecimal format
     */
    std::string get_sha256(const std::string &str);

    /**
     * template struct used as functor to get hashes
     *
     *
     * @tparam K: type of the to be hashed
     */
    template<typename K>
    struct sha256 {
        uint256 operator()(const K &key);
    };

    template<>
    struct sha256<std::string> {
        uint256 operator()(const std::string &key);
    };
}

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

#endif //HASH_TABLE_HASHFUNCTIONS_HPP
