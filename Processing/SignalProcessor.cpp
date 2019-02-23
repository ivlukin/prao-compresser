//
// Created by sorrow on 23.02.19.
//

#include <iostream>
#include "SignalProcessor.h"

void SignalProcessor::sort(const float *array[], size_t arraySize, size_t workitems) {

    int ret = -1;

    /* компилируем рабочее ядро */
    cl_kernel kernel = context.compile_kernel("Kernels/SortKernel.cl", "sort");

    /* количество work - item */
    size_t global_work_size[1] = {workitems};

    /* количество блоков */
    int memLength = static_cast<int>(global_work_size[0]);

    /* память на буфер */
    auto *mem = (cl_float *) malloc(sizeof(cl_float) * memLength * arraySize);

    /* создать буфер */
    /* TODO: положить туда массив флоатов */
    cl_mem memobj = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, memLength * sizeof(cl_float), nullptr,
                                   &ret);

    /* записать данные в буфер */
    ret = clEnqueueWriteBuffer(context.getClCommandQueue(), memobj, CL_TRUE, 0, memLength * sizeof(cl_float), mem, 0,
                               nullptr, nullptr);

    /* устанавливаем параметр */
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &memobj);


    /* выполнить кернел */
    ret = clEnqueueNDRangeKernel(context.getClCommandQueue(), kernel, 1, nullptr, global_work_size, nullptr, 0, nullptr,
                                 nullptr);

    /* считать данные из буфера */
    ret = clEnqueueReadBuffer(context.getClCommandQueue(), memobj, CL_TRUE, 0, memLength * sizeof(float), mem, 0,
                              nullptr, nullptr);

    /* данные хранятся в mem[i] */
}
