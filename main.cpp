#include "Reader.h"
#include "Calibration/test.h"
//
// Created by sorrow on 30.01.19.
//
int main() {
    Reader r = Reader("/home/sorrow/North_rays/011014_00_N1_00.pnt");
    r.readfile();

    testCalibrationStorage();
    return 0;
}

