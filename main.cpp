#include "Reader.h"
#include "Calibration/test.h"
#include "Reader/testReader.h"
#include "Processing/Sorter.h"
#include "Processing/MetricsCalculator.h"

//
// Created by sorrow on 30.01.19.
//
int main(int argc, char *argv[]) {

//
//    testCalibrationStorage();
//   testReader();
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    if (args[0] == "-f") {
        std::cout << "processing file..." << std::endl;
        GPUContext context = GPUContext();
        context.initContext();
        context.initMetricsKernels();
        size_t arraySize = 800;
        size_t arrayNum = 33 * 48;
        float array[arrayNum * arraySize];
        srand(static_cast <unsigned> (time(0)));
        for (int i = 0; i < arrayNum * arraySize; i++) {
            array[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
        clock_t tStart = clock();
        MetricsCalculator calculator = MetricsCalculator(context);
        calculator.calc(array, arrayNum, arraySize);
        std::cout << "elapsed time: " << (float) (clock() - tStart) / CLOCKS_PER_SEC << "s" << std::endl;
        return 0;
    } else if (args[0] == "-d") {
        //TODO: processing directory
        std::cout << "processing directory..." << std::endl;
    } else {
        std::cout << "ty pidor" << std::endl;
    }

}

