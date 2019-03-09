//
// Created by sorrow on 05.03.19.
//

#ifndef PRAO_COMPRESSER_PROCESSOR_H
#define PRAO_COMPRESSER_PROCESSOR_H


#include "OpenCLContext.h"

class Processor {
protected:
    OpenCLContext context = OpenCLContext();

    explicit Processor(OpenCLContext context) {
        this->context = context;
    }

    void printError(const char message[], int clError);
};


#endif //PRAO_COMPRESSER_PROCESSOR_H
