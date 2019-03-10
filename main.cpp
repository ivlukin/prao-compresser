#include "Calibration/test.h"
#include "Reader/testReader.h"
#include "Compresser.h"

//
// Created by sorrow on 30.01.19.
//
int main() {

//
//    testCalibrationStorage();
    if (false) {
        testReader();
    } else {
        OpenCLContext context = OpenCLContext();
        context.initContext();
        context.initMetricsKernels();
        clock_t tStart = clock();
        Compresser compresser = Compresser(context);
        compresser.run();
        std::cout << "elapsed time: " << (float) (clock() - tStart) / CLOCKS_PER_SEC << "s" << std::endl;
        return 0;
    }

}

