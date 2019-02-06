//
// Created by work on 04.02.2019.
//

#include "CalibrationData.h"


void CalibrationData::calculateCoefs() {
    int size = small->data.size();
    float n, Tpr;
    float *sm = small->data.data(), *bg = big->data.data();
    for (int i = 0; i < size; ++i) {
        n = bg[i] / sm[i];
        Tpr = (Tgs - n * Teq) / (n - 1);
        one_kelvin[i] = (bg[i] - sm[i]) / (Tgs - Teq);
        zero_level[i] = sm[i] - one_kelvin[i] * (Tpr + Teq);
    }

    small->data.clear();
    big->data.clear();
}

CalibrationData::CalibrationData(time_t time) : time_internal(time) { }

CalibrationData::CalibrationData(string &data1, string &data2) {
    small = new CalibrationDataInput(data1);
    big = new CalibrationDataInput(data2);

    if (small->temperature == Tgs && big->temperature == Teq)
        swap(small, big);

    if (small->time_internal != big->time_internal)
        throw logic_error("small->time_internal != big->time_internal");

    time_internal = small->time_internal;

    unsigned int size = small->data.size();
    one_kelvin = new float[size];
    zero_level = new float[size];

    calculateCoefs();
}

CalibrationData::~CalibrationData() {
    if (small != nullptr)
        delete(small);
    if (big != nullptr)
        delete(big);
    if (one_kelvin != nullptr)
        delete(one_kelvin);
    if (zero_level != nullptr)
        delete(zero_level);
    //small = big = nullptr;
}

float const * CalibrationData::get_one_kelvin(){
    return one_kelvin;
}

float const * CalibrationData::get_zero_level(){
    return zero_level;
}

void CalibrationData::print_date() {
    cout << small->datetime.tm_year + 1900 << " " << small->datetime.tm_mon + 1 << " " << small->datetime.tm_mday << " " << small->datetime.tm_hour << endl;
}

bool CalibrationData::operator<(const CalibrationData &c) const {
    return time_internal < c.time_internal;
}