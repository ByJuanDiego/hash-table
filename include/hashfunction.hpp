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


/// Group of functions and callbacks related to hashing
namespace sha2 {

    /// Transform a hexadecimal to a 256 bits integer
    uint256 to_uint256(const std::string &hex);

    /// Transform a string to his correspondent sha256 hash code
    std::string get_sha256(const std::string &str);

    /**
     * Callback struct used to get sha256 from a key.
     *
     *
     * @tparam K the type of the key to be hashed
     */
    template<typename K>
    struct sha256 {
        uint256 operator()(const K &key);
    };

    /// sha256<K> specialization for strings
    template<>
    struct sha256<std::string> {
        uint256 operator()(const std::string &key);
    };
}


#endif //HASH_TABLE_HASHFUNCTION_HPP
