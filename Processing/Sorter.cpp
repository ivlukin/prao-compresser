#include <cmath>


//
// Created by sorrow on 23.02.19.
//

#include <iostream>
#include <cmath>
#include "Sorter.h"

uint *Sorter::sort(const uint array[], size_t arraySize, size_t localWorkSize) {

    cl_int clError = -1;
    uint *toSort;
    /**
    * Размер массива
    */
    size_t m_N;
    /**
     * Размер массива, увеличенный до степени двойки
     */
    size_t m_N_padded;

    m_N = arraySize;
    m_N_padded = getPaddedSize(m_N);
    std::cout << "m_N_padded: " << m_N_padded << std::endl;
    if (m_N < m_N_padded) {
        toSort = new unsigned int[m_N_padded];
        for (int i = 0; i < m_N; ++i)
            toSort[i] = array[i];
        for (int i = m_N; i < m_N_padded; ++i)
            toSort[i] = 0;
    } else {
        toSort = new unsigned int[m_N];
        for (int i = 0; i < m_N; ++i)
            toSort[i] = array[i];
    }
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

    clError = clEnqueueWriteBuffer(context.getClCommandQueue(), m_dPingArray, CL_FALSE, 0, m_N_padded * sizeof(cl_uint),
                                   toSort, 0,
                                   nullptr, nullptr);
    printError("error writing arr to buffer", clError);


    if (m_N_padded >= localWorkSize * 2) {
        locLimit = 2 * localWorkSize;
        std::cout << "using merge sort start" << std::endl;

        // start with a local variant first, ASSUMING we have more than localWorkSize[0] * 2 elements
        clError = clSetKernelArg(context.getMergeSortStartKernel(), 0, sizeof(cl_mem), (void *) &m_dPingArray);
        printError("error setting first kernel arg", clError);

        clError |= clSetKernelArg(context.getMergeSortStartKernel(), 1, sizeof(cl_mem), (void *) &m_dPongArray);
        printError("error setting second kernel arg", clError);

        clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), context.getMergeSortStartKernel(), 1, nullptr,
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
        std::cout << "using merge sort global small" << std::endl;
        /* компилируем рабочее ядро */

        clError = clSetKernelArg(context.getMergeSortGlobalSmallKernel(), 3, sizeof(cl_uint), (void *) &m_N_padded);
        printError("failed setting 4-th arg of kernel", clError);

        for (; stride <= m_N_padded; stride <<= 1) {
            //calculate work sizes
            size_t neededWorkers = m_N_padded / stride;

            locWorkSize[0] = std::min(localWorkSize, neededWorkers);
            globalWorkSize[0] = getGlobalWorkSize(neededWorkers, localWorkSize);

            clError = clSetKernelArg(context.getMergeSortGlobalSmallKernel(), 0, sizeof(cl_mem),
                                     (void *) &m_dPingArray);
            printError("failed setting first arg of kernel", clError);
            clError |= clSetKernelArg(context.getMergeSortGlobalSmallKernel(), 1, sizeof(cl_mem),
                                      (void *) &m_dPongArray);
            printError("failed setting second arg of kernel", clError);
            clError |= clSetKernelArg(context.getMergeSortGlobalSmallKernel(), 2, sizeof(cl_uint), (void *) &stride);
            printError("failed setting third arg of kernel", clError);

            clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), context.getMergeSortGlobalSmallKernel(), 1,
                                             nullptr,
                                             globalWorkSize,
                                             locWorkSize, 0, nullptr, nullptr);
            printError("error running kernel", clError);

            std::swap(m_dPingArray, m_dPongArray);
        }
    } else {
        /* компилируем рабочее ядро */

        std::cout << "using merge sort global big" << std::endl;
        clError = clSetKernelArg(context.getMergeSortGlobalBigKernel(), 3, sizeof(cl_uint), (void *) &m_N_padded);
        printError("failed setting 4-th arg of kernel", clError);

        for (; stride <= m_N_padded; stride <<= 1) {
            //calculate work sizes
            size_t neededWorkers = m_N_padded / stride;

            locWorkSize[0] = std::min(localWorkSize, neededWorkers);
            globalWorkSize[0] = getGlobalWorkSize(neededWorkers, localWorkSize);

            clError = clSetKernelArg(context.getMergeSortGlobalBigKernel(), 0, sizeof(cl_mem), (void *) &m_dPingArray);
            printError("failed setting first arg of kernel", clError);
            clError |= clSetKernelArg(context.getMergeSortGlobalBigKernel(), 1, sizeof(cl_mem), (void *) &m_dPongArray);
            printError("failed setting second arg of kernel", clError);
            clError |= clSetKernelArg(context.getMergeSortGlobalBigKernel(), 2, sizeof(cl_uint), (void *) &stride);
            printError("failed setting third arg of kernel", clError);

            clError = clEnqueueNDRangeKernel(context.getClCommandQueue(), context.getMergeSortGlobalBigKernel(), 1,
                                             nullptr,
                                             globalWorkSize,
                                             locWorkSize, 0, nullptr, nullptr);
            printError("failed run kernel", clError);

            if (stride >= 1024 * 1024)
                std::swap(m_dPingArray, m_dPongArray);
        }
    }
    uint *tmp = new unsigned int[m_N_padded];
    clEnqueueReadBuffer(context.getClCommandQueue(), m_dPingArray, CL_TRUE, 0, m_N_padded * sizeof(cl_uint), tmp, 0,
                        nullptr,
                        nullptr);
//    uint *out = new unsigned int[m_N];
//    for (int i = 0; i < m_N_padded; ++i)
//        out[i] = tmp[m_N_padded - m_N + i];
    delete[] toSort;
    return tmp;

}

size_t Sorter::getPaddedSize(size_t n) {
    auto log2val = (unsigned int) std::ceil(std::log((float) n) / std::log(2.f));
    return (size_t) pow(2, log2val);
}

size_t Sorter::getGlobalWorkSize(size_t DataElemCount, size_t LocalWorkSize) {
    size_t r = DataElemCount % LocalWorkSize;
    if (r == 0)
        return DataElemCount;
    else
        return DataElemCount + LocalWorkSize - r;
}

