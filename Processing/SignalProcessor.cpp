#include <cmath>

#include <cmath>

#include <cmath>

//
// Created by sorrow on 23.02.19.
//

#include <iostream>
#include <cmath>
#include "SignalProcessor.h"

cl_float *SignalProcessor::sort(const cl_float array[], size_t arraySize, size_t localWorkSize) {

    cl_int clError = -1;
    cl_float out[arraySize];

    m_N = arraySize;
    m_N_padded = getPaddedSize(m_N);
    unsigned int locLimit = 1;
    size_t globalWorkSize[1];
    size_t locWorkSize[1];

    locWorkSize[0] = localWorkSize;
    globalWorkSize[0] = getGlobalWorkSize(m_N_padded / 2, localWorkSize);

    cl_mem m_dPingArray = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, sizeof(cl_uint) * m_N_padded, nullptr,
                                         &clError);
    printError("error creating input buffer", clError);

    cl_mem m_dPongArray = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, sizeof(cl_uint) * m_N_padded, nullptr,
                                         &clError);
    printError("error creating output buffer", clError);


    if (m_N_padded >= localWorkSize * 2) {
        locLimit = 2 * localWorkSize;
        cl_kernel m_MergesortStartKernel = context.compile_kernel(
                "../Kernels/AnotherSortKernel.cl",
                "Sort_MergesortStart");
        // start with a local variant first, ASSUMING we have more than localWorkSize[0] * 2 elements
        clError = clSetKernelArg(m_MergesortStartKernel, 0, sizeof(cl_mem), (void *) &array);
        printError("error setting first kernel arg", clError);

        clError |= clSetKernelArg(m_MergesortStartKernel, 1, sizeof(cl_mem), (void *) &out);
        printError("error setting second kernel arg", clError);

        clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), m_MergesortStartKernel, 1, nullptr,
                                         globalWorkSize,
                                         locWorkSize,
                                         0, nullptr, nullptr);
        printError("error running kernel", clError);

        std::swap(m_dPingArray, m_dPongArray);
    }

    locWorkSize[0] = localWorkSize;
    globalWorkSize[0] = getGlobalWorkSize(m_N_padded / 2, localWorkSize);


    // proceed with the global variant
    unsigned int stride = 2 * locLimit;


    if (m_N_padded <= MERGESORT_SMALL_STRIDE) {

        /* компилируем рабочее ядро */
        cl_kernel m_MergesortGlobalSmallKernel = context.compile_kernel(
                "../Kernels/AnotherSortKernel.cl",
                "Sort_MergesortGlobalSmall");
        clError = clSetKernelArg(m_MergesortGlobalSmallKernel, 3, sizeof(cl_uint), (void *) &m_N_padded);
        printError("failed setting 4-th arg of kernel", clError);

        for (; stride <= m_N_padded; stride <<= 1) {
            //calculate work sizes
            size_t neededWorkers = m_N_padded / stride;

            locWorkSize[0] = std::min(localWorkSize, neededWorkers);
            globalWorkSize[0] = getGlobalWorkSize(neededWorkers, localWorkSize);

            clError = clSetKernelArg(m_MergesortGlobalSmallKernel, 0, sizeof(cl_mem), (void *) &m_dPingArray);
            printError("failed setting first arg of kernel", clError);
            clError |= clSetKernelArg(m_MergesortGlobalSmallKernel, 1, sizeof(cl_mem), (void *) &m_dPongArray);
            printError("failed setting second arg of kernel", clError);
            clError |= clSetKernelArg(m_MergesortGlobalSmallKernel, 2, sizeof(cl_uint), (void *) &stride);
            printError("failed setting third arg of kernel", clError);

            clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), m_MergesortGlobalSmallKernel, 1, nullptr,
                                             globalWorkSize,
                                             locWorkSize, 0, nullptr, nullptr);
            printError("error running kernel", clError);

            std::swap(m_dPingArray, m_dPongArray);
        }
    } else {
        /* компилируем рабочее ядро */
        cl_kernel m_MergesortGlobalBigKernel = context.compile_kernel(
                "../Kernels/AnotherSortKernel.cl",
                "Sort_MergesortGlobalBig");
        clError = clSetKernelArg(m_MergesortGlobalBigKernel, 3, sizeof(cl_uint), (void *) &m_N_padded);
        printError("failed setting 4-th arg of kernel", clError);

        for (; stride <= m_N_padded; stride <<= 1) {
            //calculate work sizes
            size_t neededWorkers = m_N_padded / stride;

            locWorkSize[0] = std::min(localWorkSize, neededWorkers);
            globalWorkSize[0] = getGlobalWorkSize(neededWorkers, localWorkSize);

            clError = clSetKernelArg(m_MergesortGlobalBigKernel, 0, sizeof(cl_mem), (void *) &m_dPingArray);
            printError("failed setting first arg of kernel", clError);
            clError |= clSetKernelArg(m_MergesortGlobalBigKernel, 1, sizeof(cl_mem), (void *) &m_dPongArray);
            printError("failed setting second arg of kernel", clError);
            clError |= clSetKernelArg(m_MergesortGlobalBigKernel, 2, sizeof(cl_uint), (void *) &stride);
            printError("failed setting third arg of kernel", clError);

            clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), m_MergesortGlobalBigKernel, 1, nullptr,
                                             globalWorkSize,
                                             locWorkSize, 0, nullptr, nullptr);
            printError("failed run kernel", clError);

            if (stride >= 1024 * 1024)
                std::swap(m_dPingArray, m_dPongArray);
        }
    }
    clEnqueueReadBuffer(context.getClCommandQueue(), m_dPingArray, CL_TRUE, 0, m_N * sizeof(cl_float), out, 0, nullptr,
                        nullptr);
    return out;
}

size_t SignalProcessor::getPaddedSize(size_t n) {
    auto log2val = (unsigned int) std::ceil(std::log((float) n) / std::log(2.f));
    return (size_t) pow(2, log2val);
}

size_t SignalProcessor::getGlobalWorkSize(size_t DataElemCount, size_t LocalWorkSize) {
    size_t r = DataElemCount % LocalWorkSize;
    if (r == 0)
        return DataElemCount;
    else
        return DataElemCount + LocalWorkSize - r;
}

void SignalProcessor::printError(std::string message, int errorCode) {
    if (errorCode != 0) {
        std::cout << message << " error code:" << errorCode << std::endl;
        exit(-1);
    }
}
