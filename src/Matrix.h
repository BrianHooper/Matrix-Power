//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#ifndef MATRIX_H
#define MATRIX_H

#include <random>

class Matrix {
public:
    virtual ~Matrix() = default;

    virtual void Print() = 0;

    virtual void RandomizeAll(std::mt19937 *, double, double) = 0;

    virtual void RandomizePercentage(std::mt19937 *, double, double, double) = 0;

    virtual void Identity() = 0;

    virtual void Power(int) = 0;

    virtual std::string Type() = 0;

protected:
    int size = 0;
};

#endif //MATRIX_H
