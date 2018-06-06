//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#include <iostream>
#include <algorithm>
#include "SparseMatrix.h"

/**
 * Constructor
 * Creates an empty n x n sparse matrix
 * @param n width of the matrix
 */
SparseMatrix::SparseMatrix(int n) {
    if (n <= 0)
        return;

    size = n;
    values = new std::map<int, double>[size];
    for (int i = 0; i < size; i++) {
        values[i] = std::map<int, double>();
    }
}

/**
 * Copy constructor
 * @param other matrix to receive values from
 */
SparseMatrix::SparseMatrix(SparseMatrix *other) {
    size = other->size;
    values = new std::map<int, double>[size];
    for (int i = 0; i < size; i++) {
        std::map<int, double> row;
        auto otherRow = other->values[i].begin();
        auto last = other->values[i].end();

        while (otherRow != last) {
            int key = otherRow++->first;
            double value = other->values[i].at(key);
            row.insert(std::pair<int, double>(key, value));
        }
        values[i] = row;
    }
}

/**
 * Destructor
 */
SparseMatrix::~SparseMatrix() {
    delete[](values);
}

/**
 * Returns the type of the matrix
 * @return std::string type identifier
 */
std::string SparseMatrix::Type() {
    return "SparseMatrix";
}

/**
 * Outputs the matrix to the console
 */
void SparseMatrix::Print() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (values[i].count(j) == 1) {
                std::cout << values[i].at(j) << "\t";
            } else {
                std::cout << "0\t";
            }
        }
        std::cout << std::endl;
    }
}


/**
 * Sets a matrix equal to its identity matrix
 */
void SparseMatrix::Identity() {
    for (int i = 0; i < size; i++) {
        values[i].insert(std::pair<int, int>(i, 1));
    }
}

/**
 * Randomizes every element in the matrix
 * @param rand pointer to seeded mt19937 random object
 * @param lowerBound lowest possible value
 * @param upperBound highest possible value
 */
void SparseMatrix::RandomizeAll(std::mt19937 *rand, double lowerBound, double upperBound) {
    if (lowerBound > upperBound)
        return;

    std::uniform_real_distribution<> distribution(lowerBound, upperBound);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            values[i].insert(std::pair<int, double>(j, distribution(*rand)));
        }
    }
}

/**
 * Randomizes a percentage of the elements in the matrix
 * @param rand pointer to seeded mt19937 random object
 * @param percent % of elements to set
 * @param lowerBound lowest possible value
 * @param upperBound highest possible value
 */
void SparseMatrix::RandomizePercentage(std::mt19937 *rand, double percent, double lowerBound, double upperBound) {
    if (percent <= 0.0 || percent > 1.0)
        return;

    std::vector<std::pair<int, int>> points;
    std::uniform_real_distribution<> mValue(lowerBound, upperBound);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            points.emplace_back(std::pair<int, int>(i, j));
        }
    }
    std::shuffle(points.begin(), points.end(), *rand);
    auto numElements = (int) (percent * (size * size));
    for(int i = 0; i < numElements; i++) {

        values[points[i].first].insert(std::pair<int, double>(points[i].second, mValue(*rand)));
    }
    points.clear();
}

/**
 * Multiplies the current matrix by another matrix, storing the result in temp
 * @param other rhs of matrix multiplication
 * @param temp matrix for temporarily storing results of the calculation
 */
void SparseMatrix::Multiply(const SparseMatrix *other, SparseMatrix *temp) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Zero out any old values in the temp matrix
            if (temp->values[i].count(j) == 1) {
                temp->values[i].erase(j);
            }
            for (int k = 0; k < size; k++) {
                // Check that both the lhs and rhs of the multiplication are non-zero
                if (values[i].find(k) != values[i].end() &&
                    other->values[k].find(j) != other->values[k].end()) {
                    // Perform the multiplication
                    double result = values[i].at(k) * other->values[k].at(j);
                    // Store the result
                    if (temp->values[i].count(j) == 1) {
                        temp->values[i][j] = temp->values[i].at(j) + result;
                    } else {
                        temp->values[i].insert(std::pair<int, double>(j, result));
                    }
                }
            }
        }
    }

    // Save the results of the multiplication
    Copy(temp);
}

/**
 * Copies the elements from one matrix into the current matrix
 * @param other matrix to receive elements from
 */
void SparseMatrix::Copy(SparseMatrix *other) {
    for (int i = 0; i < size; i++) {
        values[i].clear();
        auto otherRow = other->values[i].begin();
        while (otherRow != other->values[i].end()) {
            int key = otherRow++->first;
            double value = other->values[i].at(key);
            values[i].insert(std::pair<int, double>(key, value));
        }
    }
}

/**
 * Raises the current matrix to a power
 * @param n integer power
 */
void SparseMatrix::Power(int n) {
    if (n <= 1)
        return;

    SparseMatrix original(this);
    SparseMatrix temp(size);
    for (int i = 0; i < n - 1; i++) {
        this->Multiply(&original, &temp);
    }
}

