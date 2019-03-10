//
// Created by sorrow on 04.03.19.
//

#include <iostream>
#include "MetricsCalculator.h"

metrics *MetricsCalculator::calc() {

    size_t _localWorkSize[1];
    size_t _globalWorkSize[1];

    _localWorkSize[0] = localWorkSize;
    _globalWorkSize[0] = globalWorkSize;

    int clError;

    clError = clEnqueueWriteBuffer(context.getClCommandQueue(), inputBuffer, CL_FALSE, 0,
                                   inputBufferSize,
                                   array, 0, nullptr, nullptr);

    printError("error writing input array to buffer", clError);

    clError = clSetKernelArg(context.getWorkingKernel(), 0, sizeof(cl_mem), (void *) &inputBuffer);
    printError("error setting input array (of arrays)", clError);

    clError = clSetKernelArg(context.getWorkingKernel(), 1, sizeof(cl_mem), (void *) &outBuffer);
    printError("error setting output array", clError);

    clError = clSetKernelArg(context.getWorkingKernel(), 2, sizeof(cl_int), (void *) &arraySize);
    printError("error setting array size", clError);

    clError = clSetKernelArg(context.getWorkingKernel(), 3, sizeof(cl_float), (void *) &leftPercentile);
    printError("error setting left percentile", clError);

    clError = clSetKernelArg(context.getWorkingKernel(), 4, sizeof(cl_float), (void *) &rightPercentile);
    printError("error setting right percentile", clError);

    clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), context.getWorkingKernel(), 1, nullptr,
                                     _globalWorkSize, _localWorkSize, 0, nullptr, nullptr);
    printError("error running kernel", clError);

    auto *outMetrics = new metrics[arrayNum];
    clError = clEnqueueReadBuffer(context.getClCommandQueue(), outBuffer, CL_TRUE, 0, outBufferSize, outMetrics,
                                  0, nullptr, nullptr);
    printError("error reading output buffer", clError);

    clReleaseMemObject(inputBuffer);
    clReleaseMemObject(outBuffer);
    return outMetrics;

}

void MetricsCalculator::initBuffers() {

    int clError;
    inputBufferSize = sizeof(cl_float) * arrayNum * arraySize;
    outBufferSize = sizeof(metrics) * arrayNum;

    /* input buffer. array of arrays */
    inputBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE,
                                 inputBufferSize, nullptr, &clError);
    printError("error creating input buffer", clError);

    /* out buffer. array of metrics */
    outBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, outBufferSize, nullptr, &clError);
    printError("error creating output buffer", clError);
}

