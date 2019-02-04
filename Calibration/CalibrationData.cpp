//
// Created by work on 04.02.2019.
//

#include "CalibrationData.h"


void CalibrationData::calculateCoefs() {
    int size = small->data.size();
    float n, Tpr;
    float *sm = small->data.data(), *bg = big->data.data(), *on_k = one_kelvin->data(), *zr = zero_level->data();
    for (int i = 0; i < size; ++i) {
        n = bg[i] / sm[i];
        Tpr = (Tgs - n * Teq) / (n - 1);
        on_k[i] = (bg[i] - sm[i]) / (Tgs - Teq);
        zr[i] = sm[i] - on_k[i] * (Tpr + Teq);
    }

    small->data.clear();
    big->data.clear();
}

CalibrationData::CalibrationData(time_t time) : time_internal(time) { }

CalibrationData::CalibrationData(CalibrationDataInput *data1, CalibrationDataInput *data2) {
    small = data1->temperature == Teq ? data1 : data2;
    big = data2->temperature == Tgs ? data2 : data1;
    time_internal = small->time_internal;

    unsigned int size = small->data.size();
    one_kelvin = new vector<float>(size);
    zero_level = new vector<float>(size);

    calculateCoefs();
}

const vector<float> CalibrationData::get_one_kelvin(){
    return vector<float>(*one_kelvin);
}

const vector<float> CalibrationData::get_zero_level(){
    return vector<float>(*zero_level);
}

void CalibrationData::print_date() {
    cout << small->datetime.tm_year + 1900 << " " << small->datetime.tm_mon + 1 << " " << small->datetime.tm_mday << " " << small->datetime.tm_hour << endl;
}

bool CalibrationData::operator<(const CalibrationData &c) const {
    return time_internal < c.time_internal;
}