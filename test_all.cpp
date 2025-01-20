#include "test_all.h"

#include "test_btree.h"
#include "test_hashtable.h"
#include "test_sparse_matrix.h"
#include "test_sparse_vector.h"
#include "DataStructures/BTree.h"
#include "DataStructures/HashTable.h"

#include <random>
#include <fstream>

using namespace std;
using namespace std::chrono;

void func_test_all() {
    func_test_sparse_matrix();
    func_test_sparse_vector();
    func_tests_hashtable();
    func_tests_bTree();
}

string GenerateRandomString(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t max_index = sizeof(charset) - 1;

    string result;
    result.reserve(length);

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> dist(0, max_index);

    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(generator)];
    }

    return result;
}

template <typename Dictionary>
void TestInsertionToCSV(const string& testName, const vector<size_t>& sizes, const string& filename) {
    ofstream csvFile(filename, ios::app);
    if (!csvFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    csvFile << testName;

    for (size_t num_elements : sizes) {
        Dictionary dict;

        vector<string> keys;
        keys.reserve(num_elements);
        for (size_t i = 0; i < num_elements; ++i) {
            keys.push_back(GenerateRandomString(10));
        }

        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < num_elements; ++i) {
            dict.Add(keys[i], static_cast<int>(i));
        }
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(end - start).count();

        csvFile << "," << duration;

        cout << testName << ": Inserted " << num_elements << " elements in " << duration << " ms" << endl;
    }

    csvFile << "\n";
    csvFile.close();
}

template <typename Dictionary>
void TestSearchToCSV(const string& testName, const vector<size_t>& sizes, const string& filename) {
    ofstream csvFile(filename, ios::app);
    if (!csvFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    csvFile << testName;

    for (size_t num_elements : sizes) {
        Dictionary dict;

        vector<string> keys;
        keys.reserve(num_elements);
        for (size_t i = 0; i < num_elements; ++i) {
            string key = GenerateRandomString(10);
            keys.push_back(key);
            dict.Add(key, static_cast<int>(i));
        }

        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < num_elements; ++i) {
            dict.Get(keys[i]);
        }
        auto end = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(end - start).count();

        csvFile << "," << duration;

        cout << testName << ": Searched " << num_elements << " elements in " << duration << " ms" << endl;
    }

    csvFile << "\n";
    csvFile.close();
}

void write_first(const vector<size_t>& sizes, const string& filename) {
    ofstream csvFile(filename, ios::app);
    if (!csvFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    csvFile << "Test Name";
    for(const auto& size: sizes) {
        csvFile << "," << size;
    }

    csvFile << "\n";
    csvFile.close();
}

void all_load_tests(const vector<size_t>& sizes) {
    const string filename = "performance_data.csv";

    ofstream csvFile(filename, ios::trunc);
    csvFile.close();

    write_first(sizes, filename);

    TestInsertionToCSV<BTree<string, int>>("BTree_Insertion", sizes, filename);
    TestInsertionToCSV<HashTable<string, int>>("HashTable_Insertion", sizes, filename);

    TestSearchToCSV<BTree<string, int>>("BTree_Search", sizes, filename);
    TestSearchToCSV<HashTable<string, int>>("HashTable_Search", sizes, filename);

    std::system("python3 ../plot_graph.py");
}