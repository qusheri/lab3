#pragma once

#include "DataStructures/BTree.h"
#include <iostream>
#include <string>
#include <random>
#include <cassert>
#include <chrono>

void func_tests_bTree() {

    BTree<int, std::string> table;

    table.Add(1, "test1");
    table.Add(2, "test2");
    table.Add(3, "test3");
    table.Add(4, "test4");

    assert(table.Get(1) == "test1");
    assert(table.Get(2) == "test2");
    assert(table.Get(3) == "test3");
    assert(table.Get(4) == "test4");
    assert(table.ContainsKey(2));

    table.Update(2, "test52");

    assert(table.Get(2) == "test52");

    assert(table.ContainsKey(3));
    table.Remove(3);
    assert(!table.ContainsKey(3));
    assert(table.Get(1) == "test1");
    assert(table.Get(2) == "test52");
    assert(table.Get(4) == "test4");
    assert(table.GetCount() == 3);

    std::cout << "Functional BTree tests completed.\n";
}

std::string GenerateRandomString_bTree(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t max_index = sizeof(charset) - 1;

    std::string result;
    result.reserve(length);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, max_index);

    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(generator)];
    }

    return result;
}

void load_tests_bTree(const size_t& num_elements) {
    BTree<std::string, int> bTree;

    std::vector<std::string> keys;
    keys.reserve(num_elements);

    for (size_t i = 0; i < num_elements; ++i) {
        keys.push_back(GenerateRandomString_bTree(10));
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements; ++i) {
        bTree.Add(keys[i], static_cast<int>(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to add " << num_elements << " elements: "
         << duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        if (!bTree.ContainsKey(key)) {
            std::cerr << "Error: Key not found in hash table!" << std::endl;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to find " << num_elements << " elements: "
         << duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements; ++i) {
        std::string random_key = GenerateRandomString_bTree(10);
        bTree.ContainsKey(random_key);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to check " << num_elements << " non-existing elements: "
         << duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_elements; ++i) {
        bTree.Update(keys[i], static_cast<int>(i * 2));
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to update " << num_elements << " elements: "
         << duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        bTree.Remove(key);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to remove " << num_elements << " elements: "
         << duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    if (bTree.GetCount() != 0) {
        std::cerr << "Error: Hash table is not empty after removing all elements!" << std::endl;
    } else {
        std::cout << "Hash table is empty after all operations." << std::endl;
    }
}