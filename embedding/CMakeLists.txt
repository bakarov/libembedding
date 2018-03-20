cmake_minimum_required(VERSION 3.9)
project(embedding)

set(CMAKE_CXX_STANDARD 14)

add_executable(embedding src/embedding/embedding.cpp src/embedding/embedding.h src/test/embedding_test.cpp src/paraphrase_search/paraphrase_search.cpp src/paraphrase_search/paraphrase_search.h src/embedding/cross_lang_embedding.cpp src/embedding/cross_lang_embedding.h src/embedding/mono_lang_embedding.cpp src/embedding/mono_lang_embedding.h)