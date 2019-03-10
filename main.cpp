#include "Calibration/test.h"
#include "Reader/testReader.h"
#include "Compresser.h"

//
// Created by sorrow on 30.01.19.
//
int main(int argc, char *argv[]) {

//
//    testCalibrationStorage();
    // testReader();
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    if (args[0] == "-f") {
        std::cout << "processing file..." << std::endl;
        OpenCLContext context = OpenCLContext(0, 0);
        context.initContext();
        context.initMetricsKernels();
        clock_t tStart = clock();
        Compresser compresser = Compresser("../resources/filesListBig.txt", "../resources/bigCalibrationParts.txt",
                                           context);
        compresser.run();
        std::cout << "elapsed time: " << (float) (clock() - tStart) / CLOCKS_PER_SEC << "s" << std::endl;
        return 0;
    } else if (args[0] == "-d") {
        //TODO: processing directory
        std::cout << "processing directory..." << std::endl;
    } else {
        std::cout << "ty pidor" << std::endl;
    }


}

