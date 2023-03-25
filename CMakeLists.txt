cmake_minimum_required(VERSION 3.24)
project(hash_table)

include_directories(include utils/include)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_executable(hash_table src/main.cpp include/hashtable.hpp include/hashfunction.hpp src/hashtable.cpp src/hashfunction.cpp utils/include/transaction.hpp utils/src/transaction.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        OpenSSL::SSL
        Boost::boost
)
