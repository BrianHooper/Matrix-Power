//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#ifndef LISTMATRIX_H
#define LISTMATRIX_H

#include "Matrix.h"
#include "ElementList.h"

class ListMatrix : public Matrix {
public:
    explicit ListMatrix(int);

    explicit ListMatrix(ListMatrix *);

    ~ListMatrix() override ;

    std::string Type() override;

    void Print() override;

    void RandomizeAll(std::mt19937 *, double, double) override;

    void RandomizePercentage(std::mt19937 *, double, double, double) override;

    void Identity() override;

    void Power(int) override;

private:
    void Multiply(ListMatrix *, ListMatrix *);

    ElementList values;
};

#endif // LISTMATRIX_H