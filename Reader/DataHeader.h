//
// Created by Dolgushev on 04.02.2019.
//

#ifndef PRAO_COMPRESSER_DATAHEADER_H
#define PRAO_COMPRESSER_DATAHEADER_H

#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <sstream>

#include "../Time/time.h"

using namespace std;

struct DataHeader{
    int numpar = 0, npoints = 0, nbands = 0;
    string source = "", alpha = "", delta = "";
    float fcentral = 0, wb_total = 0, tresolution = 0;
    tm begin_datetime = {}, end_datetime = {};
    int begin_time = 0, end_time = 0;
    vector<int> modulus = vector<int>();
    vector<float> wbands = vector<float>(), fbands = vector<float>();

    friend ifstream &operator>>(ifstream & in, DataHeader& dt);
};

#endif //PRAO_COMPRESSER_DATAHEADER_H
