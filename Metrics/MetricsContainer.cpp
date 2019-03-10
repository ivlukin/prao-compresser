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
    out_array_size = sizeof(metrics) / 4 * base_reader_->getPointSize() * storage.size();
    auto buffer = new float[out_array_size];


    DataHeader header = base_reader_->getDataHeader();
    int i = 0;
    for (auto it = storage.begin(); it != storage.end(); ++it){
        metrics * curr_metrics = *it;
        float * curr_out = &buffer[i * sizeof(metrics) / 4 * base_reader_->getPointSize()];

        memcpy(curr_out, curr_metrics, sizeof(metrics) * base_reader_->getPointSize());
        ++i;
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

