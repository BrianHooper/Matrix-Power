//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <map>
#include "Matrix.h"

class SparseMatrix : public Matrix {
public:
    explicit SparseMatrix(int);

    explicit SparseMatrix(SparseMatrix *);

    ~SparseMatrix() override;

    std::string Type() override;

    void Print() override;

    void Copy(SparseMatrix *);

    void RandomizeAll(std::mt19937 *, double, double) override;

    void RandomizePercentage(std::mt19937 *, double, double, double) override;

    void Identity() override;

    void Power(int) override;

private:
    void Multiply(const SparseMatrix *, SparseMatrix *);

    std::map<int, double> *values;
};

#endif // SPARSEMATRIX_H