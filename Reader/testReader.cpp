//
// Created by Dolgushev on 04.02.2019.
//

#include "testReader.h"
#include "../Calibration/CalibrationDataStorage.h"


//float arr[1000000];
/*
void testReader() {
    //DateReader dateReader("../resources/011014_01_N1_00.pnt");
    DateReader dateReader("../resources/261215_08_N1_00.pnthr");

    //string path_calibration("..\\resources\\eq_1_6b_20140101_20141231.txt");
    string path_calibration("..\\resources\\eqv_1hr.txt");
    CalibrationDataStorage storage;
    storage.add_items_from_file(path_calibration);
    //CalibrationData calibration = storage.getCalibrationData_by_date(2014, 1, 10, 8, 10);
    CalibrationData calibration = storage.getCalibrationData_by_date(2015, 12, 24, 8, 10);
    auto on_k = calibration.get_one_kelvin();
    auto zr = calibration.get_zero_level();


    //int iteration = 48 * 7;
    int iteration = 48 * 33;
    float * arr = new float[1000 * iteration];
    int end = dateReader.dataHeader.npoints * iteration;

    for (int i = 0; i < end; i += iteration) {
        dateReader.readNextPoint(arr);
        for (int j = 0; j < iteration; ++j)
            arr[j] = (arr[j] - zr[j]) / on_k[j];

        if (i % (iteration * 1000) == 0){
            end -= i;
            i = 0;
            cout << 1;
        }
    }


}
*/

CalibrationDataStorage * readCalibrationDataStorage(string path_calibration){
    float start = 0, diff = 0;
    auto *storage = new CalibrationDataStorage();

    start = clock();
    storage->add_items_from_file(path_calibration);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    cout << "reading calibration file took " << diff << " sec" << endl;

    return storage;
}

void test(string path_calibration, string path_to_datafile) {

    CalibrationDataStorage *storage = readCalibrationDataStorage(path_calibration);
    CalibrationData *calibration = storage->getCalibrationData_by_date();

    DateReader dateReader(path_to_datafile);
    dateReader.setCalibrationData(calibration);


    PointBase *point = nullptr;
    if (dateReader.getPointSize() == 48 * 7)
        point = new PointSmall();
    else if (dateReader.getPointSize() == 48 * 33)
        point = new PointBig();

    float start = 0, diff = 0;
    start = clock();
    while (!dateReader.eof()){
        dateReader.readNextPoint(point);
    }
    diff = (clock() - start) / CLOCKS_PER_SEC;

    cout << "calibration took " << diff << " sec" << endl;
}


void testReader() {
    cout << sizeof(PointBase) / (sizeof(float) * 48) << " " << sizeof(PointSmall) / (sizeof(float) * 48) << " " << sizeof(PointBig) / (sizeof(float) * 48) << endl;
    test("../resources/eq_1_6b_20140101_20141231.txt", "../resources/011014_01_N1_00.pnt");
    test("../resources/eqv_1hr.txt", "../resources/261215_08_N1_00.pnthr");

    return;
    /*
    float used_for_read_and_calibrate = 0, used_for_read_calibration = 0, start = 0, diff = 0;

    //string path_calibration("..\\resources\\eq_1_6b_20140101_20141231.txt");
    string path_calibration("..\\resources\\eqv_1hr.txt");
    CalibrationDataStorage storage;

    start = clock();
    storage.add_items_from_file(path_calibration);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    used_for_read_calibration += diff;
    cout << "reading calibration file took " << diff << " sec" << endl;


    //CalibrationData *calibration = storage.getCalibrationData_by_date(2014, 1, 10, 8, 10);
    CalibrationData *calibration = storage.getCalibrationData_by_date(2015, 12, 24, 8, 10);
    for (int t = 0; t < 100; ++t){
        //DateReader dateReader("../resources/011014_01_N1_00.pnt");
        DateReader dateReader("../resources/261215_08_N1_00.pnthr");
        dateReader.setCalibrationData(calibration);

        start = clock();
        while (!dateReader.eof()){
            //PointSmall pointSmall;
            PointBig pointSmall;
            dateReader.readNextPoint(pointSmall);
        }
        diff = (clock() - start) / CLOCKS_PER_SEC;
        used_for_read_and_calibrate += diff;
        cout << "calibration took " << diff << " sec" << endl;
    }
    cout << "total calibration took " << used_for_read_and_calibrate << " sec" << endl;
    cout << "total reading calibration file took " << used_for_read_calibration << " sec" << endl;
    */
}