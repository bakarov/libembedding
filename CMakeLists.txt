cmake_minimum_required(VERSION 3.9)
project(embedding)

set(CMAKE_CXX_STANDARD 14)

add_executable(embedding embedding.cpp embedding.h embedding_test.cpp)