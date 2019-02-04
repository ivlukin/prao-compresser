//
// Created by work on 04.02.2019.
//

#include "CalibrationDataStorage.h"


int CalibrationDataStorage::add_items_from_stream(istream &stream) {
    int count = 0;
    string line1, line2;
    while (!stream.eof()){
        getline(stream, line1, '\n');
        getline(stream, line2, '\n');

        if (line1.empty())
            break;

        auto *data1 = new CalibrationDataInput(line1), *data2 = new CalibrationDataInput(line2);

        if (data1->time_internal != data2->time_internal) {
            cout << line1 << line2;
            break;
        }

        st.insert(CalibrationData(data1, data2));
        ++count;
    }
    return count;
}

CalibrationDataStorage::CalibrationDataStorage(string &data) {
    add_items(data);
}

int CalibrationDataStorage::add_items(string &data) {
    stringstream stream(data);
    return add_items_from_stream(stream);
}


int CalibrationDataStorage::add_items_from_file(string &path) {
    ifstream stream(path);
    return add_items_from_stream(stream);
}

CalibrationData CalibrationDataStorage::getCalibrationData_by_date(int year, int mon, int day, int hour, int min, int sec) {
    year -= 1900;
    mon -= 1;
    tm time = { sec, min, hour, day, mon, year, 0, 0, 0 };
    return getCalibrationData_by_time(mktime(&time));
}

CalibrationData CalibrationDataStorage::getCalibrationData_by_time(time_t time) {
    CalibrationData to_search(time);
    auto next = st.upper_bound(to_search);
    if (next == st.begin())
        throw logic_error("too early or set is empty");
    return *(--next);
}

void CalibrationDataStorage::print() {
    for (auto i : st)
        i.print_date();
}
