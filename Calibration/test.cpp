//
// Created by work on 04.02.2019.
//

#include "test.h"

void testCalibrationStorage() {
    string path_calibration("..\\resources\\eq_1_6b_20140101_20141231.txt");
    CalibrationDataStorage storage;
    cout << "parsed: " << storage.add_items_from_file(path_calibration) << endl;
    storage.print();
    cout << endl << "found: " << endl;
    storage.getCalibrationData_by_date(2014, 1, 10, 8, 10)->print_date();
    storage.getCalibrationData_by_date(2014, 1, 1, 8, 10)->print_date();
    storage.getCalibrationData_by_date(2014, 1, 1, 8, 3)->print_date();
    storage.getCalibrationData_by_date(2014, 1, 1)->print_date();
    try {
        storage.getCalibrationData_by_date(214, 1, 10, 8, 10)->print_date();
    }
    catch (logic_error &err){
        cout << "test catching err success: " << err.what() << endl << endl;
    }
}