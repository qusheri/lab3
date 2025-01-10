cmake_minimum_required(VERSION 3.29)
project(lab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab3 main.cpp
        test_hashtable.h
        test_sparse_matrix.h
        test_sparse_vector.h
        interface.cpp
        interface.h
        test_btree.h
        test_all.h)
