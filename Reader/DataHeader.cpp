//
// Created by Dolgushev on 04.02.2019.
//

#include "DataHeader.h"

ifstream &operator>>(ifstream & in, DataHeader& dt){

    string tmp;

    in >> tmp >> dt.numpar;
    if (tmp != "numpar")
        throw logic_error("numpar not found");

    in >> tmp >> dt.source >> tmp >> dt.alpha >> tmp >> dt.delta >> tmp >> dt.fcentral >> tmp >> dt.wb_total;

    string datetime;
    in >> tmp >> datetime >> tmp >> tmp;
    sscanf(datetime.c_str(), "%d.%d.%d", &dt.begin_datetime.tm_mday, &dt.begin_datetime.tm_mon, &dt.begin_datetime.tm_year);
    in >> tmp >> datetime >> tmp >> tmp;
    sscanf(datetime.c_str(), "%d:%d:%d", &dt.begin_datetime.tm_hour, &dt.begin_datetime.tm_min, &dt.begin_datetime.tm_sec);
    dt.begin_datetime.tm_year -= 1900;
    dt.begin_datetime.tm_mon -= 1;
    dt.begin_time = mktime(&dt.begin_datetime);

    in >> tmp >> datetime;
    sscanf(datetime.c_str(), "%d.%d.%d", &dt.end_datetime.tm_mday, &dt.end_datetime.tm_mon, &dt.end_datetime.tm_year);
    in >> tmp >> datetime;
    sscanf(datetime.c_str(), "%d:%d:%d", &dt.end_datetime.tm_hour, &dt.end_datetime.tm_min, &dt.end_datetime.tm_sec);
    dt.end_datetime.tm_year -= 1900;
    dt.end_datetime.tm_mon -= 1;
    dt.end_time = mktime(&dt.end_datetime);

    in >> tmp;
    getline(in, tmp, '\n');
    stringstream stream(tmp);
    while (!stream.eof()){
        int curr;
        stream >> curr;
        dt.modulus.push_back(curr);
    }

    in >> tmp >> dt.tresolution >> tmp >> dt.npoints >> tmp >> dt.nbands;

    in >> tmp;
    getline(in, tmp, '\n');
    stream = stringstream(tmp);
    while (!stream.eof()){
        float curr;
        stream >> curr;
        dt.wbands.push_back(curr);
    }

    in >> tmp;
    getline(in, tmp, '\n');
    stream = stringstream(tmp);
    while (!stream.eof()){
        float curr;
        stream >> curr;
        dt.fbands.push_back(curr);
    }

    return in;
}