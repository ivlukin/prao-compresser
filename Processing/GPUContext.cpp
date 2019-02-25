#include <memory>

//
// Created by sorrow on 18.02.19.
//

#include <iostream>
#include <fstream>
#include "GPUContext.h"
#include <memory>

GPUContext::GPUContext() {
    int ret = -1;
    /* получение platform_id и количества платформ */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

    if (ret != 0) {
        std::cout << "fail getting platform id. ret: " << ret << std::endl;
        exit(-1);
    }

    /* получение id GPU девайса */
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    if (ret != 0) {
        std::cout << "fail getting device id. ret: " << ret << std::endl;
        exit(-1);
    }


    /* создание контекста */
    context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &ret);
    if (ret != 0) {
        std::cout << "fail creating context. ret: " << ret << std::endl;
        exit(-1);
    }

    /* создание command queue (пока не совсем понятно что это */
    command_queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &ret);
    if (ret != 0) {
        std::cout << "fail creating command_queue. ret: " << ret << std::endl;
        exit(-1);
    }
}

cl_kernel GPUContext::compile_kernel(const char filename[], const char kernelName[]) {
    cl_program program = nullptr;
    cl_kernel kernel = nullptr;

    int ret = -1;
    FILE *fp;
    size_t source_size;
    char *source_str;

    /* считывание файла с кернелом */
    try {
        fp = fopen(filename, "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        source_str = (char *) malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
    }
    catch (int a) {
        printf("%i", a);
    }

    /* создать бинарник из кода программы */
    program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size, &ret);

    /* скомпилировать программу */
    ret = clBuildProgram(program, 1, &device_id, nullptr, nullptr, nullptr);

    /* создать кернел */
    kernel = clCreateKernel(program, kernelName, &ret);

    return kernel;
}


