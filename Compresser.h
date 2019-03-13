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
    /* это нужно спросить у пользователя */
    std::string fileListPath;
    std::string calibrationListPath;
    size_t localWorkSize{};
    double starSeconds{};
    float leftPercentile{};
    float rightPercentile{};
    std::string outputPath;
public:
    Compresser(const string &fileListPath, const string &calibrationListPath, size_t localWorkSize, double starSeconds,
               float leftPercentile, float rightPercentile, const string &outputPath, const OpenCLContext &context);

private:

    /**
     * Спрашивает у пользователя
     * localWorkSize, starSeconds, leftPercentile, rightPercentile
     */
    void askData();

    /**
     * Спрашивает у пользователя
     * fileListPath
     * calibrationListPath
     */
    void askFiles();

    /* это приходит в конструкторе */
    /**
     * Хостовая часть
     */
    OpenCLContext context{};


public:
    Compresser() = default;

    explicit Compresser(OpenCLContext context);

    /**
     * Основной метод.
     * Запускает калибровку, считывание, сжатие, запись данных
     * По всем файлам из fileListPath
     */
    void run();

    /**
     * Служебный метод
     * Получает CalibrationDataStorage
     * @param path_calibration - файл с путями до файлов калибровки
     * @return CalibrationDataStorage
     */
    CalibrationDataStorage *readCalibrationDataStorage(std::string path_calibration);
};


#endif //PRAO_COMPRESSER_COMPRESSER_H
