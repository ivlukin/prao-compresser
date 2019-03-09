//
// Created by sorrow on 04.03.19.
//

#ifndef PRAO_COMPRESSER_METRICSCALCULATOR_H
#define PRAO_COMPRESSER_METRICSCALCULATOR_H


#include "OpenCLContext.h"
#include "Processor.h"

class MetricsCalculator : Processor {

public:
    /**
     * Считает метрики для массива массивов, записанных в один сплошной массив
     * @param array - массив массивов, записанный в один спложной массив
     * @param arrayNum - количество массивов
     * @param arraySize - размер одного массива
     */
    void calc(float array[], int arrayNum, int arraySize);

   explicit MetricsCalculator(OpenCLContext context) : Processor(context) {

    }
};


#endif //PRAO_COMPRESSER_METRICSCALCULATOR_H
