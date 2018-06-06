//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#include <iostream>
#include "ElementList.h"
#include <algorithm>

/**
 * Constructor
 */
ElementList::ElementList() {
	elements = std::vector<Element>();
}

/**
 * Gets the number of non-zero elements in the list
 * @return integer size
 */
int ElementList::size() {
	return length;
}

/**
 * Gets the Element object at coordinates row, col
 * @param row matrix row
 * @param col matrix column
 * @return Element(row, col, value)
 */
Element ElementList::get(int row, int col) {
	Element prototype(row, col , 0);
	for(int i = 0; i < length; i++) {
		if(prototype == elements[i]) {
			return elements[i];
		}
	}
	return {-1, -1, 0.0};
}

/**
 * Returns the value of an element in the matrix
 * @param row matrix row
 * @param col matrix column
 * @return double value at (row, col)
 */
double ElementList::valueAt(int row, int col) {
    return get(row, col).value;
}

/**
 * Increments an element in the matrix
 * @param row matrix row
 * @param col matrix column
 * @param value value to add to the element at (row, col)
 */
void ElementList::add(int row, int col, double value) {
	Element prototype(row, col , value);
	for(int i = 0; i < length; i++) {
		if(prototype == elements[i]) {
			elements[i] += prototype;
			return;
		}
	}
	insert(row, col, value);
}

/**
 * Inserts a new element into the matrix. Assumes that the previous value at
 * that location was zero.
 * @param row matrix row
 * @param col matrix col
 * @param value value to add to the element at (row, col)
 */
void ElementList::insert(int row, int col, double value){
	elements.emplace_back(row, col, value);
	length++;
}

/**
 * Retrieves an element from the list
 * @param n index of element
 * @return element at position n
 */
Element ElementList::at(int n) const {
    if(n < 0 || n >= length)
        return {-1, -1, 0};
    return elements[n];
}

/**
 * Resets the matrix to empty
 */
void ElementList::clear() {
    elements.clear();
    length = 0;
}