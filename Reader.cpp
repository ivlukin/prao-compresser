#include <utility>

//
// Created by sorrow on 29.01.19.
//

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Reader.h"

Reader::Reader(std::string filename) {
    this->filename = std::move(filename);
}

std::vector<std::string> Reader::readfile() {

    FILE *pFile;
    pFile = fopen(&this->filename[0u], "rb");
    std::vector<std::string> info;
    for (int i = 0; i < 16; i++) {
        char line[256];
        fgets(line, 256, pFile);
        std::string myLine(line);
        info.push_back(myLine);
        std::cout << myLine << std::endl;
    }
    float f;

/*    while (fscanf(pFile, "%f", f) != EOF) {
        std::cout << f << std::endl;
    }*/
    return info;
}

double Reader::calibrate(std::string values) {
    return 0;
}
