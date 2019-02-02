//
// Created by sorrow on 02.02.19.
//

#ifndef PRAO_COMPRESSER_FILEHEADER_H
#define PRAO_COMPRESSER_FILEHEADER_H

#include <string>
#include <vector>

class FileHeader {
public:
    const std::string &getSource() const;

    void setSource(const std::string &source);

    const std::string &getAlpha() const;

    void setAlpha(const std::string &alpha);

    const std::string &getDelta() const;

    void setDelta(const std::string &delta);

    float getFcentral() const;

    void setFcentral(float fcentral);

    float getWb_total() const;

    void setWb_total(float wb_total);

    const tm &getDatetime_begin() const;

    void setDatetime_begin(const tm &datetime_begin);

    const tm &getDatetime_end() const;

    void setDatetime_end(const tm &datetime_end);

    const std::vector<int> &getModulus() const;

    void setModulus(const std::vector<int> &modulus);

    float getTresolution() const;

    void setTresolution(float tresolution);

    int getNpoints() const;

    void setNpoints(int npoints);

    int getNbands() const;

    void setNbands(int nbands);

    const std::vector<float> &getFbands() const;

    void setFbands(const std::vector<float> &fbands);

private:
    std::string source;
    std::string alpha;
    std::string delta;

    /* central frequency of bandwidth */
    float fcentral;

    /* width of bandwidth */
    float wb_total;

    /* date and time of registration start */
    tm datetime_begin;

    /* date and time of registration end */
    tm datetime_end;

    /* list of active module */
    std::vector<int> modulus;

    /* time resolution */
    float tresolution;

    /* number of registered points */
    int npoints;

    /* number of bands */
    int nbands;

    /* width of every band */
    std::vector<float> wbands;
public:
    void setWbands(const std::vector<float> &wbands);

private:

    /* center of each band */
    std::vector<float> fbands;
};


#endif //PRAO_COMPRESSER_FILEHEADER_H
