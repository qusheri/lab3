#pragma once

#include "test_btree.h"
#include "test_hashtable.h"
#include "test_sparse_matrix.h"
#include "test_sparse_vector.h"

void test_all() {
    func_test_sparse_matrix();
    func_test_sparse_vector();
    func_tests_hashtable();
    func_tests_bTree();
}