//
// Created by juandiego on 3/17/23.
//

#ifndef HASH_TABLE_HASHFUNCTION_HPP
#define HASH_TABLE_HASHFUNCTION_HPP

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


#endif //HASH_TABLE_HASHFUNCTION_HPP
