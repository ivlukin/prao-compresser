//
// Created by sorrow on 05.03.19.
//

#include <iostream>
#include "Processor.h"

void Processor::printError(const char *message, int clError) {
    if (clError != 0) {
        std::cout << message << ". Error code: " << clError << std::endl;
        exit(-1);
    }
}
