//
// Created by sorrow on 04.03.19.
//

#ifndef PRAO_COMPRESSER_METRICSCALCULATOR_H
#define PRAO_COMPRESSER_METRICSCALCULATOR_H


#include "OpenCLContext.h"
#include "Processor.h"
#include "../Metrics/MetricsType.h"

class MetricsCalculator : Processor {

public:
    /**
     * Считает метрики для массива массивов, записанных в один сплошной массив
     * @param array - массив массивов, записанный в один спложной массив
     * @param arrayNum - количество массивов
     * @param arraySize - размер одного массива
     */
    metrics* calc();

    MetricsCalculator(OpenCLContext context, float *array, int arrayNum, int arraySize) : MetricsCalculator(context,
                                                                                                            array,
                                                                                                            arrayNum,
                                                                                                            arraySize,
                                                                                                            0.25f, 0.75f) {
    }

    MetricsCalculator(OpenCLContext context, float *array, int arrayNum, int arraySize, float leftPercentile,
                      float rightPercentile) : Processor(context) {
        this->array = array;
        this->arrayNum = arrayNum;
        this->arraySize = arraySize;
        this->leftPercentile = leftPercentile;
        this->rightPercentile = rightPercentile;
        globalWorkSize = static_cast<size_t>(arrayNum);
        localWorkSize = 2;
        initBuffers();
    }

private:
    cl_mem inputBuffer{};
    cl_mem outBuffer{};
    int arrayNum{};
    int arraySize{};
    float *array{};

    size_t globalWorkSize{};
    size_t localWorkSize{};
    size_t inputBufferSize{};
    size_t outBufferSize{};
    float leftPercentile{};
    float rightPercentile{};

    void initBuffers();
};


#endif //PRAO_COMPRESSER_METRICSCALCULATOR_H
