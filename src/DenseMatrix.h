//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#ifndef DENSEMATRIX_H
#define DENSEMATRIX_H

#include "Matrix.h"

class DenseMatrix : public Matrix {
public:
    explicit DenseMatrix(int);

    explicit DenseMatrix(DenseMatrix *other);

    ~DenseMatrix() override ;

    std::string Type() override;

    void Print() override;

    void Copy(DenseMatrix *);

    void RandomizeAll(std::mt19937 *, double, double) override;

    void RandomizePercentage(std::mt19937 *, double, double, double) override;

    void Identity() override;

    void Power(int) override;

private:
    void Multiply(const DenseMatrix *other, DenseMatrix *temp);

    double **values;
};

#endif // DENSEMATRIX_H