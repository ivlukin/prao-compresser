//
// Created by work on 04.02.2019.
//

#include "test.h"

void testCalibrationStorage() {
    string path_calibration("..\\resources\\smallCalibrationFull.txt");
    CalibrationDataStorage storage;
    cout << "parsed: " << storage.add_items_from_file(path_calibration) << endl;
    storage.print();

    cout << endl << "found prev: " << endl;
    storage.getCalibrationData_left_by_UTC(2014, 1, 10, 8, 10)->print_date();
    storage.getCalibrationData_left_by_UTC(2014, 1, 1, 8, 10)->print_date();
    storage.getCalibrationData_left_by_UTC(2014, 1, 1, 8, 3)->print_date();
    storage.getCalibrationData_left_by_UTC(2014, 1, 1)->print_date();
    try {
        storage.getCalibrationData_left_by_UTC(2010, 1, 10, 8, 10)->print_date();
    }
    catch (logic_error &err){
        cout << "test catching err success: " << err.what() << endl << endl;
    }

    cout << "next: " << endl;
    storage.getCalibrationData_right_by_UTC(214, 1, 10, 8, 10)->print_date();
    storage.getCalibrationData_right_by_UTC(2014, 1, 1, 8, 10)->print_date();
    storage.getCalibrationData_right_by_UTC(2014, 1, 1, 8, 3)->print_date();
    storage.getCalibrationData_right_by_UTC(2014, 1, 1)->print_date();
    try {
        storage.getCalibrationData_right_by_UTC(2050, 1, 10, 8, 10)->print_date();
    }
    catch (logic_error &err){
        cout << "test catching err success: " << err.what() << endl << endl;
    }
}