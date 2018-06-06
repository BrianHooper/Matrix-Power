//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#ifndef ELEMENTLIST_H
#define ELEMENTLIST_H

#include <vector>

#include "Element.h"


class ElementList {
public:
	ElementList();

	Element get(int, int);

    Element at(int) const;

	void add(int, int, double);

	void insert(int, int, double);

    void clear();

    double valueAt(int, int);

	int size();

private:
    int length = 0;
    
    std::vector<Element> elements;
};

#endif // ELEMENTLIST_H