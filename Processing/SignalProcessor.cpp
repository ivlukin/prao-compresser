//
// Created by sorrow on 23.02.19.
//

#include <iostream>
#include "SignalProcessor.h"

void SignalProcessor::sort(const cl_float array[], size_t arraySize, size_t workitems) {

    cl_int ret = -1;
    cl_float out[arraySize];
    cl_float aux[arraySize];

    /* компилируем рабочее ядро */
    cl_kernel kernel = context.compile_kernel(
            "/home/sorrow/CLionProjects/prao-compresser/Processing/Kernels/MergeSortKernel.cl", "MergeSort");

    /* the number of work-items you wish to execute (called the global size) */
    /* лучше всего 64 */
    size_t global_work_size[1] = {workitems};
    int memLength = global_work_size[0];


    /* создать буфер */
    cl_mem memobj = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, arraySize * sizeof(cl_float),
                                   &array,
                                   &ret);
    if (ret != 0) {
        std::cout << "fail creating the first buffer!" << " ret: " << ret << std::endl;
        exit(-1);
    }

//    /* записать данные в буфер */
    ret = clEnqueueWriteBuffer(context.getClCommandQueue(), memobj, CL_TRUE, 0,
                               sizeof(array), &array, 0,
                               nullptr, nullptr);
    if (ret != 0) {
        std::cout << "fail writing the first buffer! ret:" << ret << std::endl;
        exit(-1);
    }

    /* устанавливаем параметр */
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), memobj);

    if (ret != 0) {
        std::cout << "fail setting the first arg!" << std::endl;
        exit(-1);
    }


    memobj = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, sizeof(out) * memLength,
                            &out,
                            &ret);
    /* записать данные в буфер */
    ret = clEnqueueWriteBuffer(context.getClCommandQueue(), memobj, CL_TRUE, 0,
                               sizeof(out), &out, 0,
                               nullptr, nullptr);
    /* устанавливаем параметр */
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), memobj);

    memobj = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, sizeof(aux) * memLength,
                            &aux,
                            &ret);
    /* записать данные в буфер */
    ret = clEnqueueWriteBuffer(context.getClCommandQueue(), memobj, CL_TRUE, 0,
                               sizeof(aux), &aux, 0,
                               nullptr, nullptr);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), memobj);


    /* выполнить кернел */
    ret = clEnqueueNDRangeKernel(context.getClCommandQueue(), kernel, 1, nullptr, &global_work_size[0], nullptr, 0,
                                 nullptr,
                                 nullptr);
    clFlush(context.getClCommandQueue());
    clFinish(context.getClCommandQueue());

    /* считать данные из буфера */
    ret = clEnqueueReadBuffer(context.getClCommandQueue(), memobj, CL_TRUE, 0, sizeof(out), &out, 0,
                              nullptr, nullptr);

    for (int i = 0; i < 5; ++i)
        std::cout << out[i] << std::endl;
}
