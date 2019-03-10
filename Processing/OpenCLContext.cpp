#include <memory>

//
// Created by sorrow on 18.02.19.
//

#include <iostream>
#include <fstream>
#include "OpenCLContext.h"
#include <memory>


cl_kernel OpenCLContext::compile_kernel(const char filename[], const char kernelName[]) {
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
    if (ret != 0) {
        std::cout << "error creating binary file. ret: " << ret << std::endl;
        exit(-1);
    }

    /* скомпилировать программу */
    ret = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (ret != 0) {
        std::cout << "kernel compiling error. ret: " << ret << std::endl;
    }

    if (ret == -11) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
        exit(-1);
    }

    /* создать кернел */
    kernel = clCreateKernel(program, kernelName, &ret);
    if (ret != 0) {
        std::cout << "error creating kernel. ret: " << ret << std::endl;
        exit(-1);
    }

    return kernel;
}


void OpenCLContext::initMetricsKernels() {
    if (algorithm == 0)
        workingKernel = compile_kernel("../Processing/Kernels/nth_element.cl", "getMetrics");
    else
        workingKernel = compile_kernel("../Processing/Kernels/heapSort.cl", "getMetrics");
}

OpenCLContext &OpenCLContext::operator=(const OpenCLContext &oclContext) {
    if (this == &oclContext)
        return *this;

    platform_id = oclContext.platform_id;
    ret_num_platforms = oclContext.ret_num_platforms;
    ret_num_devices = oclContext.ret_num_devices;
    context = oclContext.context;
    command_queue = oclContext.command_queue;
    device = oclContext.device;
    workingKernel = oclContext.workingKernel;
    deviceType = oclContext.deviceType;
    algorithm = oclContext.algorithm;

    return *this;
}

void OpenCLContext::initContext() {
    std::cout << "initializating context..." << std::endl;
    int ret = -1;
    /* получение platform_id и количества платформ */
    cl_platform_id platforms[10];
    ret = clGetPlatformIDs(10, platforms, &ret_num_platforms);

    if (ret != 0) {
        std::cout << "fail getting platform id. ret: " << ret << std::endl;
        exit(-1);
    }
    if (deviceType == 0) {
        /* получение id GPU девайса */
        platform_id = platforms[0];
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device, &ret_num_devices);
        if (ret != 0) {
            std::cout << "fail getting gpu device id. ret: " << ret << std::endl;
            exit(-1);
        }
    } else {
        /* получение id CPU девайса */
        platform_id = platforms[1];
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_CPU, 1, &device, &ret_num_devices);
        if (ret != 0) {
            std::cout << "fail getting cpu device id. ret: " << ret << std::endl;
            exit(-1);
        }
    }

    /* создание контекста */
    context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &ret);
    if (ret != 0) {
        std::cout << "fail creating context. ret: " << ret << std::endl;
        exit(-1);
    }

    /* создание command queue (пока не совсем понятно что это */
    command_queue = clCreateCommandQueue(context, device, 0, &ret);
    if (ret != 0) {
        std::cout << "fail creating command_queue. ret: " << ret << std::endl;
        exit(-1);
    }
}

OpenCLContext::OpenCLContext(int deviceType, int algorithmType) {
    this->deviceType = deviceType;
    this->algorithm = algorithmType;
}


