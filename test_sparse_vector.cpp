#include <chrono>
#include <cassert>

#include "DataStructures/SparseVector.h"
#include "DataStructures/HashTable.h"
#include "DataStructures/BTree.h"
#include "test_sparse_vector.h"


double forMap_vector(const double& a) {
    return a * 2;
}

std::string forReduce_vector(const std::string& res, const double& x) {
    return std::to_string(std::stoi(res) + static_cast<int>(x));
}

void func_test_sparse_vector() {

    {
        UnqPtr<IDictionary<int, double>> dictionary(new HashTable<int, double>());
        SparseVector<double> vector(5, std::move(dictionary));

        vector.SetElement(0, 52.0);
        vector.SetElement(2, 52.52);
        vector.SetElement(4, -78.21);

        const double eps = 1e-7;
        assert(std::abs(vector.GetElement(0) - 52.0) < eps);
        assert(std::abs(vector.GetElement(1) - 0.0) < eps);
        assert(std::abs(vector.GetElement(2) - 52.52) < eps);
        assert(std::abs(vector.GetElement(3) - 0.0) < eps);
        assert(std::abs(vector.GetElement(4) + 78.21) < eps);

        vector.RemoveElement(2);
        assert(std::abs(vector.GetElement(2) - 0.0) < eps);

        vector.Map(forMap_vector);
        assert(std::abs(vector.GetElement(0) - 104.0) < eps);
        assert(std::abs(vector.GetElement(1) - 0.0) < eps);
        assert(std::abs(vector.GetElement(2) - 0.0) < eps);
        assert(std::abs(vector.GetElement(3) - 0.0) < eps);
        assert(std::abs(vector.GetElement(4) + 156.42) < eps);

        std::string start = "16";
        std::string res = vector.Reduce(forReduce_vector, start);
        assert(res == "-36");
    }

    {
        UnqPtr<IDictionary<int, double>> dictionary(new BTree<int, double>());
        SparseVector vector(5, std::move(dictionary));

        vector.SetElement(0, 52.0);
        vector.SetElement(2, 52.52);
        vector.SetElement(4, -78.21);

        const double eps = 1e-7;
        assert(std::abs(vector.GetElement(0) - 52.0) < eps);
        assert(std::abs(vector.GetElement(1) - 0.0) < eps);
        assert(std::abs(vector.GetElement(2) - 52.52) < eps);
        assert(std::abs(vector.GetElement(3) - 0.0) < eps);
        assert(std::abs(vector.GetElement(4) + 78.21) < eps);

        vector.RemoveElement(2);
        assert(std::abs(vector.GetElement(2) - 0.0) < eps);

        vector.Map(forMap_vector);
        assert(std::abs(vector.GetElement(0) - 104.0) < eps);
        assert(std::abs(vector.GetElement(1) - 0.0) < eps);
        assert(std::abs(vector.GetElement(2) - 0.0) < eps);
        assert(std::abs(vector.GetElement(3) - 0.0) < eps);
        assert(std::abs(vector.GetElement(4) + 156.42) < eps);

        std::string start = "16";
        std::string res = vector.Reduce(forReduce_vector, start);
        assert(res == "-36");
    }

    std::cout << "SparseVector tests completed.\n";
}

std::pair<double, double> load_test_sparse_vector(const int& size, const int& numElements) {
    if (size < 0) {
        std::cerr << "Error: size must be non-negative.\n";
        return{};
    }
    if (numElements < 0) {
        std::cerr << "Error: numElements must be non-negative.\n";
        return{};
    }
    std::pair<double, double> hashTableBTreeTime;

    std::cout << "Load Testing SparseVector...\n";

    {
        std::cout << "  Using HashTable as storage...\n";
        UnqPtr<IDictionary<int, double>> dictionary(new HashTable<int, double>());
        SparseVector<double> vector(size, std::move(dictionary));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numElements; ++i) {
            int index = (i * 31) % size;
            vector.SetElement(index, static_cast<double>(i));
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        hashTableBTreeTime.first = duration.count();
        std::cout << "    Time taken: " << duration.count() << " seconds\n";
    }

    {
        std::cout << "  Using BTree as storage...\n";
        UnqPtr<IDictionary<int, double>> dictionary(new BTree<int, double>());
        SparseVector<double> vector(size, std::move(dictionary));

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < numElements; ++i) {
            int index = (i * 31) % size;
            vector.SetElement(index, static_cast<double>(i));
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        hashTableBTreeTime.second = duration.count();
        std::cout << "    Time taken: " << duration.count() << " seconds\n";
    }
    std::cout << "SparseVector load tests completed.\n";
    return hashTableBTreeTime;
}