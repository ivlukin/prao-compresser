//
// Created by work on 04.02.2019.
//
#include "CalibrationDataInput.h"

CalibrationDataInput::CalibrationDataInput(string &input) {
    input = regex_replace(input, regex({R"([|,\{\}\r])"}), " ");

    istringstream in(input);
    string date, time;
    in >> date >> time >> MJD >> signal_type >> temperature >> is_data_good;

    sscanf(date.c_str(), "%d-%d-%d", &datetime.tm_year, &datetime.tm_mon, &datetime.tm_mday);
    sscanf(time.c_str(), "%d:%d:%d.", &datetime.tm_hour, &datetime.tm_min, &datetime.tm_sec);
    time_internal = tm_SubDefault(datetime);

    data = vector<float>();
    data.reserve(48 * 33);

    float curr;
    in >> curr;
    do{
        data.push_back(curr);
        curr = 0;
        in >> curr;
    } while (curr != 0);

    if (data.size() != 7 * 48 && data.size() != 33 * 48)
        throw logic_error("CalibrationDataInput read floats err");

    data.shrink_to_fit();
}