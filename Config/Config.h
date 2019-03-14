//
// Created by sorrow on 14.03.19.
//

#ifndef PRAO_COMPRESSER_CONFIG_H
#define PRAO_COMPRESSER_CONFIG_H


#include <string>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/filereadstream.h"

using namespace rapidjson;

class Config {
public:
    explicit Config(char *fileName);

private:
    std::string fileListPath;
    std::string calibrationListPath;
    size_t localWorkSize;
    double starSeconds;
    float leftPercentile;
    float rightPercentile;
    std::string outputPath;
public:
    const std::string &getFileListPath() const {
        return fileListPath;
    }

    const std::string &getCalibrationListPath() const {
        return calibrationListPath;
    }

    size_t getLocalWorkSize() const {
        return localWorkSize;
    }

    double getStarSeconds() const {
        return starSeconds;
    }

    float getLeftPercentile() const {
        return leftPercentile;
    }

    float getRightPercentile() const {
        return rightPercentile;
    }

    const std::string &getOutputPath() const {
        return outputPath;
    }
};


#endif //PRAO_COMPRESSER_CONFIG_H
