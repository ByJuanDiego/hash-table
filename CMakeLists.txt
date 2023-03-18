cmake_minimum_required(VERSION 3.24)
project(hash_table)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)

add_executable(hash_table main.cpp hashtable.h)

target_link_libraries(
    ${PROJECT_NAME}
    OpenSSL::SSL
    OpenSSL::Crypto
)
