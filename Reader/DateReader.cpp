//
// Created by Dolgushev on 04.02.2019.
//

#include "DateReader.h"

DateReader::DateReader(string path) {
    in = ifstream(path, ios::binary);
    readHeader();
}

CalibrationData* DateReader::setCalibrationData(CalibrationData *calibrationData){
    swap(calibrationData, calibration);
    return calibrationData;
}

/*
inline bool DateReader::eof() {
    return count_read_points >= dataHeader.npoints;
}
*/

void DateReader::readHeader(){
    if (!is_header_parsed){
        in >> dataHeader;
        is_header_parsed = true;
        floats_per_point = (dataHeader.nbands + 1) * 48;
    }
}

void DateReader::readNextPointInternal(float *point) {
    if (!is_header_parsed) // may be disabled for perf purposes
        readHeader();

    if (eof()) // may be disabled for perf purposes
        throw logic_error("count of points is exceed");

    in.read((char *)point, 4 * floats_per_point);
    calibratePoint(point);

    ++count_read_points;
}

void DateReader::calibratePoint(float *point) { // may be manually inlined for perf purposes
    float const *on_k = calibration->get_one_kelvin(), *zr = calibration->get_zero_level();

    for (int i = 0; i < floats_per_point; ++i)
        point[i] = (point[i] - zr[i]) / on_k[i];
}
