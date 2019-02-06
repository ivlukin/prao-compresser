//
// Created by Dolgushev on 05.02.2019.
//

#ifndef PRAO_COMPRESSER_POINT_H
#define PRAO_COMPRESSER_POINT_H

#include <istream>
#include <iostream>

using namespace std;

class PointBase {
public:
    float data[7][48];
    virtual void print(){
        cout << 1;
    }
};

class PointSmall : public PointBase {
public:
    void print() override {
        cout << 2;
    }
};

class PointBig : public PointBase  {
    float reserved[(33 - 7)][48];
public:
};


/*
class BasePoint{
public:
    static int WBANDS;
    static float TIME_PER_POINT;
    virtual void read(istream &in);
    virtual float * getData() = 0;
};

class PointSmall : public BasePoint{
public:
    static const int WBANDS = 7;
    static constexpr float TIME_PER_POINT = 0.1;
    float data[WBANDS * 48];

    float * getData() override;
};

class PointBig : public BasePoint{
public:
    static const int WBANDS = 33;
    static constexpr float TIME_PER_POINT = 0.0125;
    float data[WBANDS * 48];

    float * getData() override;
};
*/
#endif //PRAO_COMPRESSER_POINT_H
