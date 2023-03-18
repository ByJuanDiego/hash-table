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
> A second option is to create a member function at ```non_trivial_type``` definition in order to return ```hash(k.string_format())``` instead of the ```hash``` applied to a especific member variable.


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
In the examples shown above both has ```std::hash``` as hash function. As ```Hash``` is a type template parameter, it means that ```std::hash``` is not the unique hash function that can be used.
