#include "Reader.h"
#include "Calibration/test.h"
#include "Reader/testReader.h"
#include "Processing/Sorter.h"
#include "Processing/MetricsCalculator.h"

//
// Created by sorrow on 30.01.19.
//
int main() {

//
//    testCalibrationStorage();
//   testReader();
    GPUContext context = GPUContext();
    context.initMetricsKernels();
    size_t arraySize = 800;
    size_t arrayNum = 33 * 48;
    float *arrays[arrayNum];
    srand (static_cast <unsigned> (time(0)));
    for (int i = 0; i < arrayNum; i++) {
        arrays[i] = new float[arraySize];
        for (int j = 0; j < arraySize; ++j) {
            arrays[i][j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
    }
    clock_t tStart = clock();
    MetricsCalculator calculator = MetricsCalculator(context);
    calculator.calc(arrays, arrayNum, arraySize);

}

