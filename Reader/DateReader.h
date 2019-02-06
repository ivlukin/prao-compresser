//
// Created by Dolgushev on 04.02.2019.
//

#ifndef PRAO_COMPRESSER_DATEREADER_H
#define PRAO_COMPRESSER_DATEREADER_H

#include "DataHeader.h"
#include "../Point/Point.h"
#include "../Calibration/CalibrationData.h"


class DateReader{
    bool is_header_parsed = false;
    int count_read_points = 0;
    int floats_per_point;
    DataHeader dataHeader = {};

    CalibrationData *calibration;
    float *calibration_on_k = nullptr,  *calibration_zr = nullptr;

    ifstream in;


    void readHeader();
    void calibratePoint(float * point);
    void readNextPointInternal(float * to);

public:

    explicit DateReader(string filepath);
    CalibrationData * setCalibrationData(CalibrationData *calibrationData);

    /// used only for testing
    inline int getPointSize() {
        return floats_per_point;
    }

    inline bool eof() {
        return count_read_points >= dataHeader.npoints;
    }

    /// ATTENTION: function depends on offset of data array in PointBase class. data array should be placed at 0 offset (or offset 4 if class has virtual methods == is_polymorphic<PointBase>::value)
    inline void readNextPoint(PointBase *to) {
        readNextPointInternal((float*)to);
    }

    inline void readNextPoint(PointSmall &to) {
        readNextPointInternal((float*)to.data);
    }

    inline void readNextPoint(PointBig &to) {
        readNextPointInternal((float*)to.data);
    }
};

#endif //PRAO_COMPRESSER_DATEREADER_H
