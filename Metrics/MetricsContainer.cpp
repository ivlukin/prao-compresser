//
// Created by work on 10.03.2019.
//

#include "MetricsContainer.h"


void MetricsContainer::write_header(string file_path) {
    ofstream out(file_path);

    out << "numpar\t" << 1 << endl;
    out << "npoints\t" << storage.size() << endl;

    out.close();
}


float *MetricsContainer::prepare_buffer(int &out_array_size) {
    int point_size = base_reader_->getPointSize();
    int storage_size = storage.size();
    out_array_size = sizeof(metrics) / 4 * point_size * storage_size;
    auto buffer = new float[out_array_size];

    DataHeader header = base_reader_->getDataHeader();
    metrics ** data = storage.data();

    for (int k = 0; k < metrics::metric_count; ++k){ // looping metrics
        float * buff_curr = &buffer[k * point_size * storage_size];
        for (int i = 0; i < storage_size; ++i){
            for (int j = 0; j < point_size; ++j){
                buff_curr[i + j * storage_size] = ((float*)(&data[i][j]))[k];
            }
        }
    }

    return buffer;
}


MetricsContainer::MetricsContainer(DataReader *base_reader) {
    if (base_reader == nullptr)
        throw logic_error("MetricsContainer::MetricsContainer base_reader == nullptr");
    storage = vector<metrics*>();
    base_reader_ = base_reader;
}

MetricsContainer::~MetricsContainer() {
    for (auto it = storage.begin(); it != storage.end(); ++it)
        delete[] *it;
}


void MetricsContainer::addNewMetrics(metrics *metrics_) {
    storage.push_back(metrics_);
}

void MetricsContainer::saveToFile(string file_path) {
    int buffer_size;
    float * buffer_to_write = prepare_buffer(buffer_size);

    write_header(file_path);

    FILE *f = fopen(file_path.c_str(), "ab");
    fwrite(buffer_to_write, 4, buffer_size, f);

    fclose(f);
    delete[] buffer_to_write;
}

