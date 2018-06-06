//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#include <iostream>
#include <unordered_set>
#include <algorithm>
#include "ListMatrix.h"

/**
 * Constructor
 * Creates an empty n x n sparse matrix
 * @param n width of the matrix
 */
ListMatrix::ListMatrix(int n) {
    size = n;
}

/**
 * Copy constructor
 * @param other matrix to receive values from
 */
ListMatrix::ListMatrix(ListMatrix *other) {
    values = other->values;
}

/**
 * Destructor
 */
ListMatrix::~ListMatrix() = default;

/**
 * Returns the type of the matrix
 * @return std::string type identifier
 */
std::string ListMatrix::Type() {
    return "ListMatrix";
}

/**
 * Outputs the matrix to the console
 */
void ListMatrix::Print() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << values.valueAt(i, j) << "\t";
        }
        std::cout << std::endl;
    }
}

/**
 * Sets a matrix equal to its identity matrix
 */
void ListMatrix::Identity() {
    values.clear();
    for (int i = 0; i < size; i++) {
        values.insert(i, i, 1);
    }
}

/**
 * Randomizes every element in the matrix
 * @param rand pointer to seeded mt19937 random object
 * @param lowerBound lowest possible value
 * @param upperBound highest possible value
 */
void ListMatrix::RandomizeAll(std::mt19937 *rand, double lowerBound, double upperBound) {
    std::uniform_real_distribution<> mValue(lowerBound, upperBound);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            values.insert(i, j, mValue(*rand));
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
void ListMatrix::RandomizePercentage(std::mt19937 *rand, double percent, double lowerBound, double upperBound) {
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
        values.insert(points[i].first, points[i].second, mValue(*rand));
    }
}

/**
 * Multiplies the current matrix by another matrix, storing the result in temp
 * @param other rhs of matrix multiplication
 * @param temp matrix for temporarily storing results of the calculation
 */
void ListMatrix::Multiply(ListMatrix *other, ListMatrix *temp) {
    temp->values.clear();
    for (int i = 0; i < values.size(); i++) {
        for (int j = 0; j < other->values.size(); j++) {
            Element A = values.at(i);
            Element B = other->values.at(j);
            if (A.col == B.row) {
                temp->values.add(A.row, B.col, A.value * B.value);
            }
        }
    }
}

/**
 * Raises the current matrix to a power
 * @param n integer power
 */
void ListMatrix::Power(int n) {
    ListMatrix original(this);
    ListMatrix temp(size);

    for (int i = 1; i < n; i++) {
        Multiply(&original, &temp);
        values = temp.values;
    }
}



