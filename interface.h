#pragma once

#include <vector>
#include <utility>
#include "DataStructures/UnqPtr.h"
#include "DataStructures/SparseMatrix.h"
#include "DataStructures/SparseVector.h"

using Point = std::pair<double, double>;

void displayMenu();
void handleVectorOperations(UnqPtr<SparseVector<double>> &sparseVector);
void handleMatrixOperations(UnqPtr<SparseMatrix<double>> &sparseMatrix);
void plotGraphWithPython(const std::vector<Point>& points);
void printSparseVector(UnqPtr<SparseVector<double>> &sparseVector);
void printSparseMatrix(UnqPtr<SparseMatrix<double>> &sparseMatrix);