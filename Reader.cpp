#include <utility>

//
// Created by sorrow on 29.01.19.
//

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Reader.h"

Reader::Reader(std::string filename) {
    this->filename = std::move(filename);
}

std::vector<std::string> Reader::readfile() {

    /* input stream */
    FILE *pFile;
    pFile = fopen(&this->filename[0u], "rb");

    /* the first line of file - the number of lines in header */
    char numpar[6];
    int lines = 0;
    fscanf(pFile, "%s", numpar);
    fscanf(pFile, "%d", &lines);

    /* read the whole header */
    FileHeader fh = FileHeader();
    std::vector<std::string> info;
    for (int i = 0; i < lines; i++) {
        char type[16];
        char value[32];
        switch (i) { /* unfortunately, c++ doesn't support switch by string */

            case 0: {/* source */
                fscanf(pFile, "%s", type);
                fscanf(pFile, "%s", value);
                std::string source(value);
                fh.setSource(value);
                std::cout << type << value << std::endl;
                break;
            }

            case 1: {/* alpha */
                fscanf(pFile, "%s", type);
                fscanf(pFile, "%s", value);
                std::string alpha(value);
                fh.setAlpha(alpha);
                std::cout << type << value << std::endl;
                break;
            }

            case 2: { /* delta */
                fscanf(pFile, "%s", type);
                fscanf(pFile, "%s", value);
                std::string delta(value);
                fh.setDelta(delta);
                std::cout << type << value << std::endl;
                break;
            }
            case 3: {
                fscanf(pFile, "%s", type);
                float fcentral;
                fscanf(pFile, "%g", &fcentral);
                fh.setFcentral(fcentral);
                std::cout << type << fcentral << std::endl;
                break;
            }
            case 4: {
                fscanf(pFile, "%s", type);
                float wbtotal;
                fscanf(pFile, "%g", &wbtotal);
                fh.setWb_total(wbtotal);
                std::cout << type << wbtotal << std::endl;
                break;
            }
            case 5: {
                /* date begin_datetime */
                //TODO
                break;
            }
            case 6: {
                /* time begin_datetime */
                //TODO
                break;
            }
            case 7: {
                /* date end_datetime */
                //TODO
                break;
            }
            case 8: {
                /* time end_datetime */
                //TODO
                break;
            }
            case 9: {
                /* modulus */
                //TODO
                break;
            }
            case 10: {
                /* time resolution */
                fscanf(pFile, "%s", type);
                float timeResolution;
                fscanf(pFile, "%g", &timeResolution);
                fh.setTresolution(timeResolution);
                std::cout << type << timeResolution << std::endl;
                break;
            }
            case 11: {
                /* number of points */
                fscanf(pFile, "%s", type);
                int npoints;
                fscanf(pFile, "%d", &npoints);
                fh.setNpoints(npoints);
                std::cout << type << npoints << std::endl;
                break;
            }
            case 12: {
                /* number of bands */
                fscanf(pFile, "%s", type);
                int nbands;
                fscanf(pFile, "%d", &nbands);
                fh.setNbands(nbands);
                std::cout << type << nbands << std::endl;
                break;
            }
            case 13: {
                /* number of bands */
                fscanf(pFile, "%s", type);
                std::vector<float> wbands;
                for (int j = 0; j < fh.getNbands(); ++j) {
                    float width;
                    fscanf(pFile, "%g", &width);
                    wbands.push_back(width);
                }
                fh.setWbands(wbands);
                std::cout << type << std::endl;
                break;
            }
            case 14: {
                /* center of each band */
                fscanf(pFile, "%s", type);
                std::vector<float> freqs;
                for (int j = 0; j < fh.getNbands(); ++j) {
                    float freq;
                    fscanf(pFile, "%g", &freq);
                    freqs.push_back(freq);
                }
                fh.setFbands(freqs);
                std::cout << type << std::endl;
                break;
            }
        }
    }
    float f;

/*    while (fscanf(pFile, "%f", f) != EOF) {
        std::cout << f << std::endl;
    }*/
    return info;
}

double Reader::calibrate(std::string values) {
    return 0;
}

FileHeader Reader::readFileHeader(std::vector<std::string> info) {

}
