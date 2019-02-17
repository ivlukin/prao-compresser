//
// Created by Dolgushev on 04.02.2019.
//

#include "testReader.h"
#include "../Calibration/CalibrationDataStorage.h"



CalibrationDataStorage * readCalibrationDataStorage(string path_calibration){
    float start = 0, diff = 0;
    auto *storage = new CalibrationDataStorage();

    start = clock();
    storage->add_items_from_file(path_calibration);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    cout << "reading calibration file took " << diff << " sec" << endl;

    return storage;
}

float arr[1000][1584];

void testReader(){
    //string path_calibration = "../resources/smallCalibrationParts1.txt";
    string path_calibration = "../resources/bigCalibrationParts.txt";
    CalibrationDataStorage *storage = readCalibrationDataStorage(path_calibration);

    //ifstream in{R"(../resources/filesListSmall.txt)"};
    ifstream in{R"(../resources/filesListBig.txt)"};
    while (!in.eof()){
        FilesListItem item;
        in >> item;
        DataReader * reader = item.getDataReader(10);
        reader->setCalibrationData(storage);

        float start = 0, diff = 0;
        start = clock();

        int i = 0;
        try {
            while(!reader->eof())
                reader->readNextPoints(arr[0], 100);
        }
        catch (logic_error e){

        }

        diff = (clock() - start) / CLOCKS_PER_SEC;

        cout << "calibration took " << diff << " sec" << endl;
    }
}
