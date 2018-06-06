//      Brian Hooper
//      CS530 High Performance Computing
//      Lab 1
//      4/13/18

#include <iostream>
#include "Simulate.h"

int main(int argc, char *argv[]) {
    // Check for correct argument
    if (argc != 3) {
        std::cerr << "Incorrect number of parameters" << std::endl;
        return -1;
    }

    // Read file line by line and run simulation
    Simulate simulate(argv[1], argv[2]);
    simulate.run();
    return 0;
}