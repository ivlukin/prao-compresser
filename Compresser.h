//
// Created by sorrow on 10.03.19.
//

#ifndef PRAO_COMPRESSER_COMPRESSER_H
#define PRAO_COMPRESSER_COMPRESSER_H


#include <string>
#include <vector>
#include <fstream>

#include "Processing/OpenCLContext.h"
#include "Processing/MetricsCalculator.h"
#include "Calibration/CalibrationDataStorage.h"
#include "Metrics/MetricsContainer.h"
#include "Reader/FilesListItem.h"


class Compresser {
private:
    char *fileListPath;
    char *calibrationListPath;
    OpenCLContext context;
    size_t localWorkSize;


public:
    Compresser() = default;

    Compresser(char *fileListPath, char *calibrationListPath, OpenCLContext context, size_t localWorkSize);

    void run(double starSeconds, float leftPercentile, float rightPercentile);

    CalibrationDataStorage *readCalibrationDataStorage(char *path_calibration);
};


#endif //PRAO_COMPRESSER_COMPRESSER_H
