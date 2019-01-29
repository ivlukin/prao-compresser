#include <utility>

//
// Created by sorrow on 29.01.19.
//

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Reader.h"

Reader::Reader(std::string filename) {
    this->filename = std::move(filename);
}

std::string Reader::readfile() {
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << line.c_str() << std::endl;
    }

}

double Reader::calibrate(std::string values) {
    return 0;
}
