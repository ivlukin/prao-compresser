//
// Created by work on 04.02.2019.
//

#ifndef PRAO_COMPRESSER_CALIBRATIONDATA_H
#define PRAO_COMPRESSER_CALIBRATIONDATA_H

#include "CalibrationDataInput.h"
#include <iostream>

using namespace std;

/// pair of big and small parsed calibration signals with precalculated coefs used while calibration
class CalibrationData {

private:
    CalibrationDataInput *small = nullptr, *big = nullptr;
    vector<float> *one_kelvin, *zero_level;
    time_t time_internal;

/// precalculates one_kelvin and zero_level coefs used while calibration
    void calculateCoefs();

public:
    static const int Tgs = 2400, Teq = 278; /// temperature of big signal and of small signal

    /// used to create instances to make searches by value "time"
    CalibrationData(time_t time);

    /// small and big parsed calibration signals
    CalibrationData(CalibrationDataInput *data1, CalibrationDataInput *data2);

    const vector<float> get_one_kelvin();
    const vector<float> get_zero_level();

    void print_date();

    bool operator<(const CalibrationData & c) const;
};

#endif //PRAO_COMPRESSER_CALIBRATIONDATA_H
