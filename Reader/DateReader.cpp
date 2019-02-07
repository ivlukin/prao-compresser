//
// Created by Dolgushev on 04.02.2019.
//

#include "DateReader.h"

DateReader::DateReader(string path) {
    in = ifstream(path, ios::binary | ios::in);
    readHeader();
    buffer = new char[BUFFER_SIZE];
    in.read(buffer, BUFFER_SIZE);
}

DateReader::~DateReader() {
    delete(buffer);
    in.close();
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

    if (buffer_pointer == BUFFER_SIZE){
        buffer_pointer = 0;
        in.read(buffer, BUFFER_SIZE);
        calibrateArrayPoints((float*)buffer);
    }

    memcpy(point, &buffer[buffer_pointer], 4 * floats_per_point);
    buffer_pointer += 4 * floats_per_point;

    //calibratePoint(point);

    ++count_read_points;
}

void DateReader::calibrateArrayPoints(float *point) {
    float const *on_k = calibration->get_one_kelvin(), *zr = calibration->get_zero_level();

    for (int i = 0; i < BUFFER_SIZE / (floats_per_point * 4); ++i)
        for (int j = 0; j < floats_per_point; ++j)
            point[i * floats_per_point + j] = (point[i * floats_per_point + j] - zr[j]) / on_k[j];
}
/*
void DateReader::calibratePoint(float *point) { // may be manually inlined for perf purposes
    float const *on_k = calibration->get_one_kelvin(), *zr = calibration->get_zero_level();

    for (int i = 0; i < floats_per_point; ++i)
        point[i] = (point[i] - zr[i]) / on_k[i];
}
*/