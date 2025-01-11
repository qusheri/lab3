#pragma once

#include <iostream>

double forMap_matrix(const double& a);

std::string forReduce_matrix(const std::string& res, const double& x);

void func_test_sparse_matrix();

std::pair<double, double> load_test_sparse_matrix(const int& size, const int& numElements);