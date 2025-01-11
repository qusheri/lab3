#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

#include "interface.h"
#include "test_all.h"
#include "DataStructures/HashTable.h"
#include "DataStructures/BTree.h"

void displayMenu() {
    std::cout << "Select action:" << std::endl;
    std::cout << "1. Interact with structs\n";
    std::cout << "2. Funcional tests\n";
    std::cout << "3. Load tests with graphs\n";
    std::cout << "4. Exit\n";
    int choice;
    std::cin >> choice;

    if(choice == 2) {
        func_test_all();
    } else if(choice == 3) {
        std::vector<size_t> sizes;
        for(size_t i = 1; i <= 10001; i+= 1000) {
            sizes.push_back(i);
        }
        all_load_tests(sizes);
    } else if(choice == 1) {
        int structureChoice = 0;
        int dictionaryChoice = 0;

        std::cout << "Select Structure:\n";
        std::cout << "1. Sparse Vector\n";
        std::cout << "2. Sparse Matrix\n";
        std::cin >> structureChoice;

        std::cout << "Select Dictionary:\n";
        std::cout << "1. HashTable\n";
        std::cout << "2. BTree\n";
        std::cin >> dictionaryChoice;

        if (structureChoice == 1) {
            int length;
            std::cout << "Enter length of the dictionary:\n";
            std::cin >> length;
            UnqPtr<IDictionary<int, double>> dictionary;
            if (dictionaryChoice == 1) {
                dictionary = UnqPtr<IDictionary<int, double>>(new HashTable<int, double>());
            } else if (dictionaryChoice == 2) {
                dictionary = UnqPtr<IDictionary<int, double>>(new BTree<int, double>());
            } else {
                std::cout << "Invalid choice. Exiting...\n";
                return;
            }

            auto sparseVector = UnqPtr(new SparseVector(length, std::move(dictionary)));
            handleVectorOperations(sparseVector);

        } else if (structureChoice == 2) {
            int rows, cols;
            std::cout << "Enter rows and columns:\n";
            std::cin >> rows >> cols;
            UnqPtr<IDictionary<IndexPair, double>> dictionary;
            if (dictionaryChoice == 1) {
                dictionary = UnqPtr<IDictionary<IndexPair, double>>(new HashTable<IndexPair, double>());
            } else if (dictionaryChoice == 2) {
                dictionary = UnqPtr<IDictionary<IndexPair, double>>(new BTree<IndexPair, double>());
            } else {
                std::cout << "Invalid choice. Exiting...\n";
                return;
            }

            auto sparseMatrix = UnqPtr(new SparseMatrix(rows, cols, std::move(dictionary)));
            handleMatrixOperations(sparseMatrix);
        } else {
            std::cout << "Invalid choice. Exiting...\n";
        }
    } else {
        std::cout << "Exiting...\n";
    }
}

void handleVectorOperations(UnqPtr<SparseVector<double>> &sparseVector) {
    while (true) {
        int choice;
        std::cout << "\nSparse Vector Operations:\n";
        std::cout << "1. Set Element\n";
        std::cout << "2. Get Element\n";
        std::cout << "3. Print Elements\n";
        std::cout << "4. Exit\n";
        std::cin >> choice;

        if (choice == 1) {
            int index;
            double value;
            std::cout << "Enter index: ";
            std::cin >> index;
            if(index >= sparseVector->GetLength()) {
                std::cout << "Invalid index. Exiting...\n";
                return;
            }
            std::cout << "Enter value: ";
            std::cin >> value;

            sparseVector->SetElement(index, value);
        } else if (choice == 2) {
            int index;
            std::cout << "Enter index: ";
            std::cin >> index;
            if(index >= sparseVector->GetLength()) {
                std::cout << "Invalid index. Exiting...\n";
                return;
            }
            try {
                double value = sparseVector->GetElement(index);
                std::cout << "Value at index " << index << ": " << value << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        } else if (choice == 3) {
            printSparseVector(sparseVector);
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void handleMatrixOperations(UnqPtr<SparseMatrix<double>> &sparseMatrix) {
    while (true) {
        int choice;
        std::cout << "\nSparse Matrix Operations:\n";
        std::cout << "1. Set Element\n";
        std::cout << "2. Get Element\n";
        std::cout << "3. Print Elements\n";
        std::cout << "4. Exit\n";
        std::cin >> choice;

        if (choice == 1) {
            int row, col;
            double value;
            std::cout << "Enter row: ";
            std::cin >> row;
            std::cout << "Enter column: ";
            std::cin >> col;
            if(row >= sparseMatrix->GetRows() || col >= sparseMatrix->GetColumns()) {
                std::cout << "Invalid index. Exiting...\n";
                return;
            }
            std::cout << "Enter value: ";
            std::cin >> value;

            sparseMatrix->SetElement(row, col, value);
        } else if (choice == 2) {
            int row, col;
            std::cout << "Enter row: ";
            std::cin >> row;
            std::cout << "Enter column: ";
            std::cin >> col;
            if(row >= sparseMatrix->GetRows() || col >= sparseMatrix->GetColumns()) {
                std::cout << "Invalid index. Exiting...\n";
                return;
            }
            try {
                double value = sparseMatrix->GetElement(row, col);
                std::cout << "Value at (" << row << "," << col << "): " << value << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        } else if (choice == 3) {
            printSparseMatrix(sparseMatrix);
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}

void plotGraphWithPython(const std::vector<Point>& points) {
    std::ofstream outFile("points.csv");
    if (!outFile) {
        std::cerr << "Error: Unable to create points.csv\n";
        return;
    }

    for (const auto& point : points) {
        outFile << point.first << "," << point.second << "\n";
    }
    outFile.close();

    std::string command = "python3 ../plot_graph.py points.csv";
    int ret_code = std::system(command.c_str());
    if (ret_code != 0) {
        std::cerr << "Error: Failed to execute Python script\n";
    }
}

void printSparseVector(UnqPtr<SparseVector<double>> &sparseVector) {
    std::cout << "SparseVector elements:\n";
    for(int i = 0; i < sparseVector->GetLength(); ++i) {
        std::cout << sparseVector->GetElement(i) << " ";
    }
    std::cout << '\n';
}

void printSparseMatrix(UnqPtr<SparseMatrix<double>> &sparseMatrix) {
    std::cout << "SparseMatrix elements:\n";
    for(int i = 0; i < sparseMatrix->GetRows(); ++i) {
        for (int j = 0; j < sparseMatrix->GetColumns(); ++j) {
            std::cout << sparseMatrix->GetElement(i, j) << " ";
        }
        std::cout << "\n";
    }
}