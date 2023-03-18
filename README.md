# Hash table C++ implementation

## Hash table template parameters 

```c++
template<typename KT, typename KV, typename Hash = std::hash<KT>, class KeyEqual = std::equal_to<KT>>
class hash_table;
```

- ```KT```: key type of the hash table
- ```KV```: key valuy of the hash table
- ```Hash```: is a function type that overloads ```() operator``` that recieves a ```KT``` and returns an unsigned long which is the key hash
- ```KeyEqual```: is a function type that overloads ```() operator``` to define when two keys have the same value

### Usage cases

1. Using the ```hash_table``` with key types provided with template specializations (such as ```int```, ```long```, ```unsigned```, ```std::string```, etc)

```c++
hash_table<basic_type, any_value_type> hashTable;
```

> As the key type is a basic type it has specializations for ```std::hash``` and ```std::equal_to```; as both are default parameters, it is not necesary to specify the types for ```Hash``` and ```KeyEqual``` respectively

2. Using the ```hash_table``` with a user defined type

```c++
hash_table<non_trivial_type, any_value_type> hashTable;
```

> The ```STL``` do not have a template specialization for this user defined type; this is why the code below needs to be written.

```c++
// specialization for std::hash
template<>
std::hash<non_trivial_type> {
    unsigned long operator() (const non_trivial_type& k) {
        std::hash<basic_type> hash;
        return hash(k.basic_type_member_variable);
    }
}
```
> A second option is to create a member function at ```non_trivial_type``` definition in order to return ```hash(k.string_format())``` instead of the ```hash``` applied to a especific member variable. For more information, visit [cppreference/hash](https://en.cppreference.com/w/cpp/utility/hash)


```c++
// specialization for std::equals_to
template<>
std::equals_to<non_trivial_type> {
    bool operator(const non_trivial_type& a, const non_trivial_type& b) {
        std::equals_to<comparable_type> equals;
        return equals(a.comparable_member_variable, b.comparable_member_variable); // equivalent to a.x == b.x
    }
}
```

> Note that if ```non_trivial_type``` have $k$ comparable member variables, depending of the implementation requirements, the function could need to verify if the $k$ values are equal in ```a``` and ```b```

Once finished the template specialization the ```hash_table``` instance should work properly.

## Third party hash functions
In the examples shown above both ```KT``` has ```std::hash``` as hash function. As ```Hash``` is a type template parameter, it means that ```std::hash``` is not the unique hash function that can be used. For example, another hash function is ```SHA256```, popular by being one of the most secure hash algorithm used by some of the most secure networks in the world. ```OpenSSL``` is one of the third party libraries that provides this algorithm.

To install OpenSSL in Fedora Linux 37
```
sudo yum install openssl
sudo yum install openssl-devel
```

To link the library with the project add the following lines to the ```CMakeList.txt``` file
```cmake
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
```

```SHA256``` algorithm would look like this:
```c++
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
```
> As we can see, sha256 can only recieve a ```std::string``` and return another ```std::string```, this will be important later

And the hash function interface to be used in the instanciation
```c++
template<typename KT>
struct hash_sha256 {
    unsigned long operator()(const KT &key) {
        std::hash<KT> h;
        return h(sha256(key));
    }
};
```
If ```key_type``` is not an ```std::string```, is required a template specialization for ```key_type``` in order to guarantee that sha256 is recieving a ```std::string```. An usage example is:
```c++
template<>
struct hash_sha256<non_string_type> {
    unsigned long operator()(const non_string_type &key) {
        std::hash<std::string> h;
        return h(sha256(key.string_format()));
    }
};
```

Finally, all the ingredients are ready to create a new ```hash_table```:
```c++
hash_table<key_type, any_value_type, hash_sha256<key_type>> hashTable;
```
> do not forget to define ```std::equal<key_type>``` if needed, independant of the hash function
