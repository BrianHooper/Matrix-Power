//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#include <iostream>
#include <algorithm>
#include "DenseMatrix.h"

/**
 * Constructor
 * Creates an empty n x n sparse matrix
 * @param n width of the matrix
 */
DenseMatrix::DenseMatrix(int n) {
    if (n <= 0)
        return;
    size = n;
    values = new double *[size];

    for (int i = 0; i < size; i++) {
        values[i] = new double[size];
        for (int j = 0; j < size; j++) {
            values[i][j] = 0;
        }
    }
}

/**
 * Copy constructor
 * @param other matrix to receive values from
 */
DenseMatrix::DenseMatrix(DenseMatrix *other) {
    size = other->size;

    values = new double *[size];

    for (int i = 0; i < size; i++) {
        values[i] = new double[size];
        for (int j = 0; j < size; j++) {
            values[i][j] = other->values[i][j];
        }
    }
}

/**
 * Destructor
 */
DenseMatrix::~DenseMatrix() {
    for(int i = 0; i < size; i++) {
        delete[](values[i]);
    }
    delete[](values);
}

/**
 * Outputs the matrix to the console
 */
void DenseMatrix::Print() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << values[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

/**
 * Returns the type of the matrix
 * @return std::string type identifier
 */
std::string DenseMatrix::Type() {
    return "DenseMatrix";
}

/**
 * Randomizes every element in the matrix
 * @param rand pointer to seeded mt19937 random object
 * @param lowerBound lowest possible value
 * @param upperBound highest possible value
 */
void DenseMatrix::RandomizeAll(std::mt19937 *rand, double lowerBound, double upperBound) {
    std::uniform_real_distribution<> distribution(lowerBound, upperBound);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            values[i][j] = distribution(*rand);
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
void DenseMatrix::RandomizePercentage(std::mt19937 *rand, double percent, double lowerBound, double upperBound) {
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

        values[points[i].first][points[i].second] = mValue(*rand);
    }
    points.clear();
}

/**
 * Sets a matrix equal to its identity matrix
 */
void DenseMatrix::Identity() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                values[i][j] = 1;
            } else {
                values[i][j] = 0;
            }
        }
    }
}

/**
 * Copies the elements from one matrix into the current matrix
 * @param other matrix to receive elements from
 */
void DenseMatrix::Copy(DenseMatrix *other) {
    // Copy the temp matrix into the current matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            values[i][j] = other->values[i][j];
        }
    }
}

/**
 * Multiplies the current matrix by another matrix, storing the result in temp
 * @param other rhs of matrix multiplication
 * @param temp matrix for temporarily storing results of the calculation
 */
void DenseMatrix::Multiply(const DenseMatrix *other, DenseMatrix *temp) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            temp->values[i][j] = 0;
            for (int k = 0; k < size; k++) {
                temp->values[i][j] += values[i][k] * other->values[k][j];
            }
        }
    }

    Copy(temp);
}

/**
 * Raises the current matrix to a power
 * @param n integer power
 */
void DenseMatrix::Power(int n) {
    if (n <= 1)
        return;

    DenseMatrix original(this);
    DenseMatrix temp(size);
    for (int i = 0; i < n - 1; i++) {
        this->Multiply(&original, &temp);
    }
}
