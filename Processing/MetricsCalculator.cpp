//
// Created by sorrow on 04.03.19.
//

#include <iostream>
#include "MetricsCalculator.h"

void MetricsCalculator::calc(float *array[], int arrayNum, int arraySize) {
    std::cout << "starting calculating..." << std::endl;
    cl_int clError;
    size_t globalWorkSize[1];
    size_t localWorkSize[1];

    globalWorkSize[0] = arrayNum * 4;
    localWorkSize[0] = 16;

    /* input buffer. array of arrays */
    cl_mem inputBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE,
                                        sizeof(cl_float) * globalWorkSize[0] * arraySize,
                                        nullptr, &clError);
    printError("error creating input buffer", clError);

    /* out buffer. array of dispersions */
    cl_mem outBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * arrayNum,
                                      nullptr, &clError);
    printError("error creating output buffer", clError);

    clError = clEnqueueWriteBuffer(context.getClCommandQueue(), inputBuffer, CL_FALSE, 0,
                                   sizeof(cl_float) * globalWorkSize[0] * arraySize,
                                   array, 0, nullptr, nullptr);
    printError("error writing input array to buffer", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 0, sizeof(inputBuffer), (void *) &inputBuffer);
    printError("error setting input array (of arrays)", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 1, sizeof(outBuffer), (void *) &outBuffer);
    printError("error setting output array", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 2, sizeof(cl_int), (void *) &arraySize);
    printError("error setting third arg", clError);

    clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), context.getMetricsKernel(), 1, nullptr,
                                     globalWorkSize, localWorkSize, 0, nullptr, nullptr);
    printError("error running kernel", clError);

    float *out = new float[arrayNum];
    clError = clEnqueueReadBuffer(context.getClCommandQueue(), outBuffer, CL_TRUE, 0, sizeof(cl_float) * arrayNum, out,
                                  0,
                                  nullptr,
                                  nullptr);
    printError("error reading buffer", clError);


}

