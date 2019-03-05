//
// Created by sorrow on 04.03.19.
//

#ifndef PRAO_COMPRESSER_METRICSCALCULATOR_H
#define PRAO_COMPRESSER_METRICSCALCULATOR_H


#include "GPUContext.h"
#include "Processor.h"

class MetricsCalculator : Processor {

public:
    void calc(float *array[], int arrayNum, int arraySize);

    explicit MetricsCalculator(GPUContext context) : Processor(context) {

    }
};


#endif //PRAO_COMPRESSER_METRICSCALCULATOR_H
