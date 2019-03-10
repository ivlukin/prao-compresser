//
// Created by Dolgushev on 04.02.2019.
//

#ifndef PRAO_COMPRESSER_DATEREADER_H
#define PRAO_COMPRESSER_DATEREADER_H

#include "DataHeader.h"
#include "../Point/Point.h"
#include "../Calibration/CalibrationDataStorage.h"

#include <cstring>

class DataReader{
    bool is_header_parsed = false;
    int count_read_points = 0, points_before_switch_calibration = 0;
    int floats_per_point = 0, size_per_point = 0;
    float points_per_chunk = 0, remainder = 0; // how much point the time chunk consists of; the remainder while reading integer of points_per_chunk
    DataHeader dataHeader = {};

    CalibrationDataStorage *calibration;
    double *calibration_on_k = nullptr, *on_k_step = nullptr, *calibration_zr = nullptr, *zr_step = nullptr;

    ifstream in;
    static constexpr int BUFFER_SIZE = 0x1000 * 33 * 7 * 3 * 4; // disk sector size + count of big channels + count of small channels + 48 modules + coef ~11MB
    int buffer_pointer = BUFFER_SIZE;
    char *buffer = nullptr;

    double timeChunk_duration_star = 0, timeChunk_duration_sun = 0;


    void readHeader();
    void realloc(double *& base, double const * from);
    void updateCalibrationData();
    void calibrateArrayPoints(float *point, int count);
    void calibrateArrayPointsDetailed(float *point, int size);
    void readNextPointsInternal(float *point, int full_count, int offset, int local_count);
public:

    explicit DataReader(string filepath, double starSeconds_timeChunk_dur);
    ~DataReader();
    void setCalibrationData(CalibrationDataStorage *calibrationData);

    inline int getNeedBufferSize(){
        return floats_per_point * int(points_per_chunk + 1);
    }
    /// used only for testing
    inline int getPointSize() {
        return floats_per_point;
    }

    inline float get_MJD_begin(){
        return dataHeader.MJD_begin;
    }

    inline bool eof() {
        return count_read_points >= dataHeader.npoints;
    }
/*
    /// ATTENTION: function depends on offset of data array in PointBase class. data array should be placed at 0 offset (or offset 4 if class has virtual methods == is_polymorphic<PointBase>::value)
    inline void readNextPoint(PointBase *to) {
        readNextPoints((float *) to);
    }

    inline void readNextPoint(PointSmall &to) {
        readNextPoints((float *) to.data);
    }

    inline void readNextPoint(PointBig &to) {
        readNextPoints((float *) to.data);
    }
    */
    int readNextPoints(float *to);
    void readNextPoints(float *to, int count);
};

#endif //PRAO_COMPRESSER_DATEREADER_H
