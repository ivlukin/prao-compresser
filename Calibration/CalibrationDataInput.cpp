//
// Created by work on 04.02.2019.
//
#include "CalibrationDataInput.h"

CalibrationDataInput::CalibrationDataInput(string &input) {
    int start;
    while(start = min(min(input.find('|'), input.find(',')), min(input.find('{'), input.find('}'))), start > 0)
        input[start] = ' ';

    istringstream in(input);
    string date, time;
    in >> date >> time >> unk1 >> unk2 >> temperature >> unk3;

    sscanf(date.c_str(), "%d-%d-%d", &datetime.tm_year, &datetime.tm_mon, &datetime.tm_mday);
    sscanf(time.c_str(), "%d:%d:%d.", &datetime.tm_hour, &datetime.tm_min, &datetime.tm_sec);
    datetime.tm_year -= 1900;
    datetime.tm_mon -= 1;
    time_internal = mktime(&datetime);

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