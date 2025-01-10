#pragma once

#include "DataStructures/SparseMatrix.h"
#include "DataStructures/HashTable.h"
#include "DataStructures/BTree.h"
#include <iostream>
#include <chrono>
#include <cassert>

double forMap_matrix(const double& a) {
    return a * 2;
}

std::string forReduce_matrix(const std::string& res, const double& x) {
    return std::to_string(std::stoi(res) + static_cast<int>(x));
}

inline void func_test_sparse_matrix() {

    {
        UnqPtr<IDictionary<IndexPair, double>> dictionary(new HashTable<IndexPair, double>());
        SparseMatrix matrix(3, 3, std::move(dictionary));

        assert(matrix.GetColumns() == 3);
        assert(matrix.GetRows() == 3);

        matrix.SetElement(0, 0, 1.0);
        matrix.SetElement(1, 1, 2.0);
        matrix.SetElement(2, 2, 3.0);
        matrix.SetElement(2, 1, 4.0);

        const double epsilon = 1e-6;
        assert(std::abs(matrix.GetElement(0, 0) - 1.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 1) - 2.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 2) - 3.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 1) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 1) - 4.0) < epsilon);

        matrix.RemoveElement(0, 0);
        assert(std::abs(matrix.GetElement(0, 0) - 0.0) < epsilon);

        matrix.Map(forMap_matrix);
        assert(std::abs(matrix.GetElement(0, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 1) - 4.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 2) - 6.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 1) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 1) - 8.0) < epsilon);

        std::string start = "0";
        std::string res = matrix.Reduce(forReduce_matrix, start);
        assert(res == "18");
    }

    {
        UnqPtr<IDictionary<IndexPair, double>> dictionary(new BTree<IndexPair, double>());
        SparseMatrix matrix(3, 3, std::move(dictionary));

        assert(matrix.GetColumns() == 3);
        assert(matrix.GetRows() == 3);

        matrix.SetElement(0, 0, 1.0);
        matrix.SetElement(1, 1, 2.0);
        matrix.SetElement(2, 2, 3.0);
        matrix.SetElement(2, 1, 4.0);

        const double epsilon = 1e-6;
        assert(std::abs(matrix.GetElement(0, 0) - 1.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 1) - 2.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 2) - 3.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 1) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 1) - 4.0) < epsilon);

        matrix.RemoveElement(0, 0);
        assert(std::abs(matrix.GetElement(0, 0) - 0.0) < epsilon);

        matrix.Map(forMap_matrix);
        assert(std::abs(matrix.GetElement(0, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 1) - 4.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 2) - 6.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 1) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(0, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(1, 2) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 0) - 0.0) < epsilon);
        assert(std::abs(matrix.GetElement(2, 1) - 8.0) < epsilon);

        std::string start = "0";
        std::string res = matrix.Reduce(forReduce_matrix, start);
        assert(res == "18");
    }

    std::cout << "SparseMatrix tests completed.\n";
}

inline std::pair<double, double> load_test_sparse_matrix(const int& size, const int& numElements) {
    if (size < 0) {
        std::cerr << "Error: size must be non-negative.\n";
        return {};
    } else if (numElements < 0) {
        std::cerr << "Error: numElements must be non-negative.\n";
        return{};
    }
    std::pair<double, double> hashTableBTreeTime;
    std::cout << "Load Testing SparseMatrix...\n";

    {
        std::cout << "  Using HashTable as storage...\n";
        UnqPtr<IDictionary<IndexPair, double>> dictionary(new HashTable<IndexPair, double>());
        SparseMatrix<double> matrix(size, size, std::move(dictionary));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numElements; ++i) {
            int row = i % size;
            int col = (i * 31) % size;
            matrix.SetElement(row, col, static_cast<double>(i));
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        hashTableBTreeTime.first = duration.count();
        std::cout << "    Time taken: " << duration.count() << " seconds\n";
    }

    {
        std::cout << "  Using BTree as storage...\n";
        UnqPtr<IDictionary<IndexPair, double>> dictionary(new BTree<IndexPair, double>());
        SparseMatrix<double> matrix(size, size, std::move(dictionary));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numElements; ++i) {
            int row = i % size;
            int col = (i * 31) % size;
            matrix.SetElement(row, col, static_cast<double>(i));
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        hashTableBTreeTime.second = duration.count();
        std::cout << "    Time taken: " << duration.count() << " seconds\n";
    }

    std::cout << "SparseMatrix load tests completed.\n";
    return hashTableBTreeTime;
}