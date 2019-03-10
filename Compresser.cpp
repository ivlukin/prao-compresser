//
// Created by sorrow on 10.03.19.
//

#include <fstream>
#include "Compresser.h"
#include "Calibration/CalibrationDataStorage.h"
#include "Reader/FilesListItem.h"
#include "Metrics/MetricsType.h"
#include "Processing/MetricsCalculator.h"

Compresser::Compresser(char *fileListPath, char *calibrationListPath, OpenCLContext context, size_t localWorkSize) {
    this->context = context;
    this->fileListPath = fileListPath;
    this->calibrationListPath = calibrationListPath;
    this->localWorkSize = localWorkSize;
}


void Compresser::run(double starSeconds, float leftPercentile, float rightPercentile) {
    /* get calibration files */
    CalibrationDataStorage *storage = readCalibrationDataStorage(calibrationListPath);
    ifstream in(fileListPath);
    while (!in.eof()) {
        FilesListItem item;
        in >> item;
        DataReader *reader = item.getDataReader(starSeconds);
        auto *data_reordered_buffer = new float[reader->getNeedBufferSize()];
        reader->setCalibrationData(storage);
        int count;
        int pointSize;
        int i = 1;
        try {
            clock_t tStart = clock();
            while (!reader->eof()) {
                count = reader->readNextPoints(data_reordered_buffer);
                pointSize = reader->getPointSize();
                MetricsCalculator
                        calculator = MetricsCalculator(context, data_reordered_buffer, reader->getPointSize(), count,
                                                       localWorkSize,
                                                       leftPercentile,
                                                       rightPercentile);
                auto *metrics_buffer = calculator.calc();
                if (i % 30 == 0)
                    std::cout << i << " arrays calculated..." << std::endl;
                /* пока так. но вообще то это надо записывать. */
                delete[] metrics_buffer;
                ++i;
            }
            std::cout << "calculating work time: " << (float) (clock() - tStart) / (float) CLOCKS_PER_SEC << "s"
                      << std::endl;
        }
        catch (logic_error e) {
            std::cout << e.what() << std::endl;
            std::cout << "count: " << count << std::endl;
            std::cout << "point size: " << pointSize << std::endl;
        }

    }
}

CalibrationDataStorage *Compresser::readCalibrationDataStorage(char *path_calibration) {
    float start = 0, diff = 0;
    auto *storage = new CalibrationDataStorage();

    start = clock();
    std::string path(path_calibration);
    storage->add_items_from_file(path);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    cout << "reading calibration file took " << diff << " sec" << endl;
    return storage;
}
