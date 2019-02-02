//
// Created by sorrow on 29.01.19.
//

#ifndef PRAO_COMPRESSER_READER_H
#define PRAO_COMPRESSER_READER_H

#include <string>
#include <vector>
#include "FileHeader.h"


class Reader {
public:
    std::vector<std::string> readfile();

    double calibrate(std::string values);

    explicit Reader(std::string filename);

private:
    FileHeader readFileHeader(std::vector<std::string>  info);
    std::string filename;

};

#endif //PRAO_COMPRESSER_READER_H

