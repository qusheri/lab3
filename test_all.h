#pragma once

#include <vector>
#include <chrono>

void func_test_all();

std::string GenerateRandomString(size_t length);

template <typename Dictionary>
void TestInsertionToCSV(const std::string& testName, const std::vector<size_t>& sizes, const std::string& filename);

template <typename Dictionary>
void TestSearchToCSV(const std::string& testName, const std::vector<size_t>& sizes, const std::string& filename);

void all_load_tests(const std::vector<size_t>& sizes);