//
// Created by brian on 4/16/18.
//

#ifndef SIMULATE_H
#define SIMULATE_H

#include <fstream>
#include <random>
#include "Matrix.h"

class Simulate {
public:
    typedef struct {
        int matrix = 0;
        int size = 0;
        int power = 0;
        double lowerBound = 0.0;
        double upperBound = 0.0;
        double sparsity = 0.0;
        int timeFunction = 0;
    } parameters;

    Simulate(const char *inputFilename, const char *outputFilename);

    ~Simulate();

    void run();
private:

    void parseInput(const std::string *line);

    bool validBounds(parameters);

    bool convertTokens(const std::vector<std::string>*, parameters*);

    std::string type(Matrix *);

    Matrix *createMatrix(int, int);

    void fill(Matrix *, double, double, double);

    void fill(Matrix *, double, double);

    void fill(Matrix *);

    double raise(Matrix *, int, int);

    double chronoTime(Matrix *, int);

    double systemTime(Matrix *, int);

    double clockTime(Matrix *, int);

    std::mt19937 rand;
    std::ofstream outputFile;
    std::vector<parameters> inputParameters;
};


#endif //SIMULATE_H
