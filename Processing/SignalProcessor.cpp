//
// Created by sorrow on 18.02.19.
//

#include <iostream>
#include <fstream>
#include "SignalProcessor.h"
#include <memory>

SignalProcessor::SignalProcessor() {
    int ret = -1;
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueueWithProperties(context, device_id, &properties, &ret);
}

cl_kernel SignalProcessor::compile_kernel(const char *filename) {
    cl_program program = NULL;
    cl_kernel kernel = NULL;

    int ret = -1;
    size_t source_size;
    std::unique_ptr<char[]> source;
    char *source_str;

    try {
        std::ifstream stream;
        stream.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
        stream.open(filename, std::ios_base::in);
        stream.seekg(0, std::ios_base::end);
        source_size = stream.tellg();
        stream.seekg(0, std::ios_base::beg);
        source.reset(new char[source_size]);
        stream.read(source.get(), source_size);
    } catch (const std::exception &e) {
        std::cerr << "Can't read kernel: " << e.what() << std::endl;
        exit(1);
    }

/* создать бинарник из кода программы */
    program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size, &ret);

/* скомпилировать программу */
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

/* создать кернел */
    kernel = clCreateKernel(program, "test", &ret);

    return kernel;
}
