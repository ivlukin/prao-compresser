#include "Reader.h"
#include "Calibration/test.h"
#include "Reader/testReader.h"
#include "Processing/Sorter.h"

//
// Created by sorrow on 30.01.19.
//
int main() {
//    Reader r = Reader("/home/sorrow/North_rays/011014_00_N1_00.pnt");
//    r.readfile();

    // testCalibrationStorage();
    // testReader();
    GPUContext context = GPUContext();
    context.initSortKernels();
    size_t size = 800;
    uint arr[size];
    srand((unsigned int) time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i] = rand();
    }
    clock_t tStart = clock();
    Sorter processor = Sorter(context);
    uint *result = processor.sort(arr, size, 256);
    clock_t tFinish = clock();
    for (int i = 0; i < size; ++i)
        std::cout << i << ": " << result[i] << std::endl;
    std::cout << std::endl;
    delete[] result;
    printf("Time taken: %.2fs\n", (double) (tFinish- tStart) / CLOCKS_PER_SEC);
    return 0;
}

