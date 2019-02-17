//
// Created by work on 10.02.2019.
//

#ifndef PRAO_COMPRESSER_FILESLISTITEM_H
#define PRAO_COMPRESSER_FILESLISTITEM_H

#include <ctime>
#include <string>
#include <regex>

#include "../Time/Time.h"
#include "DataReader.h"

using namespace std;

struct FilesListItem{
    tm time_UTC = {};
    string filepath = string();
    int nbands = 0, npoints = 0;
    double tresolution = 0, star_time_start = 0, star_time_end = 0, time_JD = 0;
    DataReader *reader = nullptr;

    DataReader* getDataReader(double starSeconds_timeChunk_dur);

    friend istream &operator>>(istream & in, FilesListItem& dt);
};

#endif //PRAO_COMPRESSER_FILESLISTITEM_H
