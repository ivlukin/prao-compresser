//
// Created by sorrow on 10.03.19.
//

#ifndef PRAO_COMPRESSER_COMPRESSER_H
#define PRAO_COMPRESSER_COMPRESSER_H


#include <string>
#include <vector>
#include "Processing/OpenCLContext.h"
#include "Calibration/CalibrationDataStorage.h"

class Compresser {
private:
    char *fileListPath;
    char *calibrationListPath;
    OpenCLContext context;


public:
    Compresser() = default;

    Compresser(char *fileListPath, char *calibrationListPath, OpenCLContext context);

    void run(double starSeconds, float leftPercentile, float rightPercentile);

    CalibrationDataStorage *readCalibrationDataStorage(char *path_calibration);
};


#endif //PRAO_COMPRESSER_COMPRESSER_H
