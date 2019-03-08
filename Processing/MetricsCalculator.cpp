//
// Created by sorrow on 04.03.19.
//

#include <iostream>
#include "MetricsCalculator.h"

void MetricsCalculator::calc(float array[], int arrayNum, int arraySize) {
    std::cout << "starting calculating metrics..." << std::endl;

    cl_int clError;
    size_t globalWorkSize[1];
    size_t localWorkSize[1];

    localWorkSize[0] = 2;
    globalWorkSize[0] = arrayNum;

    int inputBufferSize = sizeof(cl_float) * arrayNum * arraySize;
    int outBufferSize = sizeof(cl_float) * arrayNum;

    /* input buffer. array of arrays */
    cl_mem inputBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE,
                                        inputBufferSize, nullptr, &clError);
    printError("error creating input buffer", clError);

    /* out buffer. array of dispersions */
    cl_mem outDispBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, outBufferSize, nullptr, &clError);
    /* out buffer. array of means */
    cl_mem outMeanBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, outBufferSize, nullptr, &clError);
    printError("error creating output buffer", clError);

    clError = clEnqueueWriteBuffer(context.getClCommandQueue(), inputBuffer, CL_FALSE, 0,
                                   inputBufferSize,
                                   array, 0, nullptr, nullptr);

    printError("error writing input array to buffer", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 0, sizeof(inputBuffer), (void *) &inputBuffer);
    printError("error setting input array (of arrays)", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 1, sizeof(outDispBuffer), (void *) &outDispBuffer);
    printError("error setting dispersion output array", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 2, sizeof(outMeanBuffer), (void *) &outMeanBuffer);
    printError("error setting mean output array", clError);

    clError = clSetKernelArg(context.getMetricsKernel(), 3, sizeof(cl_int), (void *) &arraySize);
    printError("error setting array size", clError);

    clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), context.getMetricsKernel(), 1, nullptr,
                                     globalWorkSize, localWorkSize, 0, nullptr, nullptr);
    printError("error running kernel", clError);

    float outDispersion[arrayNum];
    clError = clEnqueueReadBuffer(context.getClCommandQueue(), outDispBuffer, CL_TRUE, 0, outBufferSize, outDispersion,
                                  0, nullptr, nullptr);
    printError("error reading disp buffer", clError);

    float outMean[arrayNum];
    clError = clEnqueueReadBuffer(context.getClCommandQueue(), outMeanBuffer, CL_TRUE, 0, outBufferSize, outMean,
                                  0, nullptr, nullptr);
    printError("error reading mean buffer", clError);

}

