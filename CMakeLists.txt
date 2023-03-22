cmake_minimum_required(VERSION 3.24)
project(hash_table)

set(CMAKE_CXX_STANDARD 20)
configure_file(assets/transactions.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_executable(hash_table main.cpp include/hashtable.hpp utils/hashfunctions.hpp)

target_link_libraries(
        ${PROJECT_NAME}
        OpenSSL::SSL
        Boost::boost
)
