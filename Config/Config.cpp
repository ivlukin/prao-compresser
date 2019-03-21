//
// Created by sorrow on 14.03.19.
//

#include <iostream>
#include "Config.h"

Config::Config(char *fileName) {
    std::cout<<"parsing config..." << std::endl;
    FILE *fp = fopen(fileName, "r"); // non-Windows use "r"
    char readBuffer[2048];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    fclose(fp);

    assert(d.HasMember("fileListPath"));
    assert(d.HasMember("calibrationListPath"));
    assert(d.HasMember("localWorkSize"));
    assert(d.HasMember("starSeconds"));
    assert(d.HasMember("leftPercentile"));
    assert(d.HasMember("rightPercentile"));
    assert(d.HasMember("outputPath"));
    assert(d.HasMember("algorithm"));

    this->fileListPath = d["fileListPath"].GetString();
    this->calibrationListPath = d["calibrationListPath"].GetString();
    this->localWorkSize = static_cast<size_t>(d["localWorkSize"].GetInt());
    this->starSeconds = d["starSeconds"].GetDouble();
    this->leftPercentile = d["leftPercentile"].GetFloat();
    this->rightPercentile = d["rightPercentile"].GetFloat();
    this->outputPath = d["outputPath"].GetString();
    this->algorithm = d["algorithm"].GetInt();
}
