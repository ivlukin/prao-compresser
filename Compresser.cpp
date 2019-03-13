#include <utility>

//
// Created by sorrow on 10.03.19.
//

#include "Compresser.h"

Compresser::Compresser(OpenCLContext context) {
    this->context = context;
    askData();
    askFiles();
}


void Compresser::run() {
    /* get calibration files */
    CalibrationDataStorage *storage = readCalibrationDataStorage(calibrationListPath);
    ifstream in(fileListPath);
    while (!in.eof()) {
        FilesListItem item;
        in >> item;
        DataReader *reader = item.getDataReader(starSeconds);
        auto *data_reordered_buffer = new float[reader->getNeedBufferSize()];
        reader->setCalibrationData(storage);

        MetricsContainer container(reader);

        int i = 1;
        clock_t tStart, sum = 0;
        try {
            while (!reader->eof()) {
                int count = reader->readNextPoints(data_reordered_buffer);
                MetricsCalculator
                        calculator = MetricsCalculator(context, data_reordered_buffer, reader->getPointSize(), count,
                                                       localWorkSize,
                                                       leftPercentile,
                                                       rightPercentile);
                tStart = clock();
                auto *metrics_buffer = calculator.calc();
                container.addNewMetrics(metrics_buffer);
                sum += clock() - tStart;

                if (i % 30 == 0)
                    std::cout << i << " arrays calculated..." << std::endl;
                ++i;
            }
            std::cout << "calculating work time: " << (float) (sum) / (float) CLOCKS_PER_SEC << "s"
                      << std::endl;

            string dst_directory = item.filepath;
            container.saveToFile(dst_directory + ".processed");
        }
        catch (logic_error e) {
            std::cout << e.what() << std::endl;
        }


    }
}

CalibrationDataStorage *Compresser::readCalibrationDataStorage(std::string path_calibration) {
    float start = 0, diff = 0;
    auto *storage = new CalibrationDataStorage();

    start = clock();
    std::string path(std::move(path_calibration));
    storage->add_items_from_file(path);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    cout << "reading calibration file took " << diff << " sec" << endl;
    return storage;
}

void Compresser::askData() {
    std::cout << "enter left percentile (0.02 for example):" << std::endl;
    std::cin >> leftPercentile;
    std::cout << "enter right percentile (0.7 for example):" << std::endl;
    std::cin >> rightPercentile;
    std::cout << "enter star seconds (10 for example):" << std::endl;
    std::cin >> starSeconds;
    std::cout << "enter local work size (2 for example):" << std::endl;
    std::cin >> localWorkSize;
}

void Compresser::askFiles() {
    std::cout << "enter full path to file containing list of files:" << std::endl;
    std::cin >> fileListPath;
    std::cout << "enter full path to file containing calibration info" << std::endl;
    std::cin >> calibrationListPath;
}

Compresser::Compresser(const string &fileListPath, const string &calibrationListPath, size_t localWorkSize,
                       double starSeconds, float leftPercentile, float rightPercentile, const string &outputPath,
                       const OpenCLContext &context) {
    this->calibrationListPath = calibrationListPath;
    this->fileListPath = fileListPath;
    this->localWorkSize = localWorkSize;
    this->starSeconds = starSeconds;
    this->rightPercentile = rightPercentile;
    this->leftPercentile = leftPercentile;
    this->outputPath = outputPath;
}
