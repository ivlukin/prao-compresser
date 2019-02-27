#include "Reader.h"
#include "Calibration/test.h"
#include "Reader/testReader.h"
#include "Processing/SignalProcessor.h"

//
// Created by sorrow on 30.01.19.
//
int main() {
//    Reader r = Reader("/home/sorrow/North_rays/011014_00_N1_00.pnt");
//    r.readfile();

    // testCalibrationStorage();
    // testReader();
    size_t size = 800;
    cl_float arr[size];
    for (int i = 0; i < size; i++) {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        arr[i] = (cl_float) r;
    }
    for (int i = 0; i < 5; ++i)
        std::cout << arr[i] << std::endl;
    std::cout << "========" << std::endl;
    SignalProcessor processor = SignalProcessor();
    processor.sort(arr, size, 256);
    return 0;
}

