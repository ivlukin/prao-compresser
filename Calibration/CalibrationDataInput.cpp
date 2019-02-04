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

    vector<float> data = vector<float>(33 * 48);
    float curr;
    while(!in.eof()) {
        in >> curr;
        data.push_back(curr);
    }

    data.shrink_to_fit();
}