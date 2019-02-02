//
// Created by sorrow on 02.02.19.
//

#include "FileHeader.h"

const std::string &FileHeader::getSource() const {
    return source;
}

void FileHeader::setSource(const std::string &source) {
    FileHeader::source = source;
}

const std::string &FileHeader::getAlpha() const {
    return alpha;
}

void FileHeader::setAlpha(const std::string &alpha) {
    FileHeader::alpha = alpha;
}

const std::string &FileHeader::getDelta() const {
    return delta;
}

void FileHeader::setDelta(const std::string &delta) {
    FileHeader::delta = delta;
}

float FileHeader::getFcentral() const {
    return fcentral;
}

void FileHeader::setFcentral(float fcentral) {
    FileHeader::fcentral = fcentral;
}

float FileHeader::getWb_total() const {
    return wb_total;
}

void FileHeader::setWb_total(float wb_total) {
    FileHeader::wb_total = wb_total;
}

const tm &FileHeader::getDatetime_begin() const {
    return datetime_begin;
}

void FileHeader::setDatetime_begin(const tm &datetime_begin) {
    FileHeader::datetime_begin = datetime_begin;
}

const tm &FileHeader::getDatetime_end() const {
    return datetime_end;
}

void FileHeader::setDatetime_end(const tm &datetime_end) {
    FileHeader::datetime_end = datetime_end;
}

const std::vector<int> &FileHeader::getModulus() const {
    return modulus;
}

void FileHeader::setModulus(const std::vector<int> &modulus) {
    FileHeader::modulus = modulus;
}

float FileHeader::getTresolution() const {
    return tresolution;
}

void FileHeader::setTresolution(float tresolution) {
    FileHeader::tresolution = tresolution;
}

int FileHeader::getNpoints() const {
    return npoints;
}

void FileHeader::setNpoints(int npoints) {
    FileHeader::npoints = npoints;
}

int FileHeader::getNbands() const {
    return nbands;
}

void FileHeader::setNbands(int nbands) {
    FileHeader::nbands = nbands;
}


const std::vector<float> &FileHeader::getFbands() const {
    return fbands;
}

void FileHeader::setFbands(const std::vector<float> &fbands) {
    FileHeader::fbands = fbands;
}

void FileHeader::setWbands(const std::vector<float> &wbands) {
    FileHeader::wbands = wbands;
}
