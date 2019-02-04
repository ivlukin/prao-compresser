//
// Created by work on 04.02.2019.
//

#ifndef PRAO_COMPRESSER_CALIBRATIONDATASTORAGE_H
#define PRAO_COMPRESSER_CALIBRATIONDATASTORAGE_H

#include "CalibrationData.h"
#include <set>
#include <fstream>

/// stores all known calibrations and extracts appropriate by datetime
class CalibrationDataStorage{
private:
    set<CalibrationData> st;

    int add_items_from_stream(istream &stream);

public:
    CalibrationDataStorage() = default;

    /// array of non-parsed calibration signals in text
    explicit CalibrationDataStorage(string &data);

    /// array of non-parsed calibration signals in text
    int add_items(string &data);

    /// read file and add to storage
    int add_items_from_file(string &path);

    /// get appropriate pre-calculated calibration signals by date
    CalibrationData getCalibrationData_by_date(int year=2000, int mon=0, int day=0, int hour=0, int min=0, int sec=0);

    /// get appropriate pre-calculated calibration signals by time
    CalibrationData getCalibrationData_by_time(time_t time);

    void print();
};

#endif //PRAO_COMPRESSER_CALIBRATIONDATASTORAGE_H
