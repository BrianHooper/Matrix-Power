//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <fstream>
#include <sstream>

#include "Simulate.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "ListMatrix.h"

/**
 * Converts a list of strings to parameters
 * @param tokens pointer to std::vector<str::string> object
 * @param p parameters struct
 * @return true if all values are valid numbers
 */
bool Simulate::convertTokens(const std::vector<std::string>* tokens, parameters* p) {
    // Check number of inputs
    if (tokens->size() != 7) {
        std::cerr << "Incorrect number of parameters in input file" << std::endl;
        return false;
    }

    // Attempt to convert string tokens to numbers
    try {
        p->matrix = std::stoi(tokens->at(0));
        p->size = std::stoi(tokens->at(1));
        p->lowerBound = std::stod(tokens->at(2));
        p->upperBound = std::stod(tokens->at(3));
        p->sparsity = std::stod(tokens->at(4));
        p->timeFunction = std::stoi(tokens->at(5));
        p->power = std::stoi(tokens->at(6));
    } catch (std::invalid_argument &e) {
        std::cerr << "Error parsing file" << std::endl;
        return false;
    } catch (std::out_of_range &e) {
        std::cerr << "Error parsing file" << std::endl;
        return false;
    }
    return true;
}

/**
 * Checks the bounds on a set of input parameters
 * @param p parameters struct
 * @return true if all values are within bounds
 */
bool Simulate::validBounds(parameters p) {
    if (p.matrix < 0 || p.matrix > 2)
        return false;
    if (p.size <= 0)
        return false;
    if (p.lowerBound < 0.0 || p.upperBound < p.lowerBound)
        return false;
    if (p.sparsity < 0.0 || p.sparsity > 1.0)
        return false;
    if (p.timeFunction < 0 || p.timeFunction > 2)
        return false;
    if (p.power <= 1)
        return false;

    return true;
}

/**
 * Parses an input string into matrix parameters
 * @param line std::string containing csv values
 */
void Simulate::parseInput(const std::string *line) {
    // Ignore comments
    if (line->length() == 0 || line->at(0) == '#') {
        return;
    }

    // Split input into tokens
    std::vector<std::string> tokens;
    std::istringstream stm(*line);
    std::string token;
    while (std::getline(stm, token, ',')) {
        tokens.push_back(token);
    }

    parameters p;
    if(convertTokens(&tokens, &p) && validBounds(p)) {
        inputParameters.push_back(p);
    }
}


/**
 * Constructor, seeds mt19937 random function, reads input file, opens output file for writing
 */
Simulate::Simulate(const char *inputFilename, const char *outputFilename) {
    std::random_device seedRandom;
    rand = std::mt19937(seedRandom());

    // open file for reading
    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: file " << inputFilename << " cannot be opened." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        parseInput(&line);
    }
    inputFile.close();

    // open file for writing
    outputFile = std::ofstream(outputFilename, std::ofstream::out | std::ofstream::app);
    if (!outputFile.is_open()) {
        std::cerr << "Error: file " << outputFilename << " cannot be opened." << std::endl;
        return;
    }
}

/**
 * Destructor
 */
Simulate::~Simulate() {
    if (outputFile.is_open()) {
        outputFile.close();
    }
}

/**
 *  Performs each matrix multiplication given by the input file
 */
void Simulate::run() {
    if (!outputFile.is_open()) {
        std::cerr << "Error: output file is not accessible." << std::endl;
        return;
    }

    for (parameters p : inputParameters) {
        Matrix *matrix = createMatrix(p.matrix, p.size);
        fill(matrix, p.lowerBound, p.upperBound, p.sparsity);
        std::cout << "Calculating " << matrix->Type() << std::endl;
        if (p.size <= 10) {
            matrix->Print();
        } 
        double time = raise(matrix, p.timeFunction, p.power);

        std::string results = type(matrix) + "," + std::to_string(p.size) + "," + std::to_string(p.sparsity)
                              + "," + std::to_string(p.power) + "," + std::to_string(time);
        outputFile << results << std::endl;
        delete (matrix);
    }
}

/**
 * Returns the type of the matrix
 * @return std::string type identifier
 */
std::string Simulate::type(Matrix *matrix) {
    if (matrix == nullptr) {
        return "None";
    } else {
        return matrix->Type();
    }
}

/**
 * Creates an n x n matrix of type DenseMatrix, SparseMatrix, or ListMatrix, depending on type
 *
 * type 0: DenseMatrix
 * type 1: SparseMatrix
 * type 2: ListMatrix
 * @param type class of matrix
 * @param n width of matrix
 */
Matrix *Simulate::createMatrix(int type, int n) {
    switch (type) {
        case 0:
            return new DenseMatrix(n);
        case 1:
            return new SparseMatrix(n);
        case 2:
            return new ListMatrix(n);
        default:
            return nullptr;
    }
}

/**
 * Completely fills the matrix with values between the lowerBound and upperBound, inclusive
 * @param lowerBound smallest value
 * @param upperBound largest value
 */
void Simulate::fill(Matrix *matrix, double lowerBound, double upperBound) {
    if (matrix == nullptr) {
        std::cerr << "Error, matrix not initialized." << std::endl;
        return;
    }

    matrix->RandomizeAll(&rand, lowerBound, upperBound);
}

/**
 * Sets the matrix equal to its identity matrix
 */
void Simulate::fill(Matrix *matrix) {
    if (matrix == nullptr) {
        std::cerr << "Error, matrix not initialized." << std::endl;
        return;
    }

    matrix->Identity();
}

/**
 * Partially fills a matrix
 * @param lowerBound smallest value
 * @param upperBound largest value
 * @param sparsity percentage of elements to set
 */
void Simulate::fill(Matrix *matrix, double lowerBound, double upperBound, double sparsity) {
    if (matrix == nullptr) {
        std::cerr << "Error, matrix not initialized." << std::endl;
        return;
    }

    if (sparsity == 1.0) {
        fill(matrix, lowerBound, upperBound);
    } else if (sparsity == 0.0) {
        fill(matrix);
    } else {
        matrix->RandomizePercentage(&rand, sparsity, lowerBound, upperBound);
    }
}

/**
 * Raises the matrix to the nth power and records the time using chrono
 * @param power exponent value
 * @return time elapsed in milliseconds
 */
double Simulate::chronoTime(Matrix *matrix, int power) {
    auto start = ((double) std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count()) / 1000;

    matrix->Power(power);

    auto stop = ((double) std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count()) / 1000;
    return stop - start;
}

/**
 * Raises the matrix to the nth power and records the time using timeval struct
 * @param power exponent value
 * @return time elapsed in milliseconds
 */
double Simulate::systemTime(Matrix *matrix, int power) {
    struct timeval tp{};
    gettimeofday(&tp, nullptr);
    auto start = (double) (tp.tv_sec * 1000L + tp.tv_usec / 1000) / 1000;

    matrix->Power(power);

    gettimeofday(&tp, nullptr);
    auto stop = (double) (tp.tv_sec * 1000L + tp.tv_usec / 1000) / 1000;
    return stop - start;
}

/**
 * Raises the matrix to the nth power and records the time using clock cycles
 * @param power exponent value
 * @return time elapsed in seconds
 */
double Simulate::clockTime(Matrix *matrix, int power) {
    clock_t start = clock();

    matrix->Power(power);

    clock_t stop = clock();
    return (double) ((stop - start) / CLOCKS_PER_SEC);
}

/**
 * Raises the matrix to a power using various methods of recording time
 * type 0: chrono time
 * type 1: timeval struct
 * type 2: clock cycles
 * @param type specifies which time function to use
 * @param power exponent value
 * @return time elapsed in milliseconds
 */
double Simulate::raise(Matrix *matrix, int type, int power) {
    if (matrix == nullptr) {
        std::cerr << "Error, matrix not initialized." << std::endl;
        return 0.0;
    }

    switch (type) {
        case 0:
            return chronoTime(matrix, power);
        case 1:
            return systemTime(matrix, power);
        case 2:
            return clockTime(matrix, power);
        default:
            return 0.0;
    }
}