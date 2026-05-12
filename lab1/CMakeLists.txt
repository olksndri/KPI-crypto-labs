cmake_minimum_required(VERSION 3.10.0)

project(crypto_lab VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE CIPHER_SRC "cipher/*.cpp")

add_executable(crypto_lab 
    main.cpp
    lang.cpp
    ${CIPHER_SRC}
)

