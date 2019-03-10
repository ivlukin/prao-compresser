//
// Created by work on 10.03.2019.
//

#ifndef PRAO_COMPRESSER_METRICSCONTAINER_H
#define PRAO_COMPRESSER_METRICSCONTAINER_H

#include <string>
#include <vector>
#include <fstream>

#include "MetricsType.h"
#include "../Reader/DataReader.h"

class MetricsContainer {
    vector<metrics *> storage;
    DataReader *base_reader_;

    float * prepare_buffer(int &out_array_size);
    void write_header(string file_path);
public:

    MetricsContainer(DataReader *base_reader);
    ~MetricsContainer();
    void addNewMetrics(metrics * metrics_);
    void saveToFile(string file_path);
};


#endif //PRAO_COMPRESSER_METRICSCONTAINER_H
