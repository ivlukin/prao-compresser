//
// Created by sorrow on 18.02.19.
//
/* this is opencl sandbox */
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#ifndef PRAO_COMPRESSER_GPUCONTEXT_H
#define PRAO_COMPRESSER_GPUCONTEXT_H

#define MAX_SOURCE_SIZE (0x100000)


class GPUContext {
public:
    GPUContext();

    /**
     * Собирает и компилирует ядро
     * @param filename - путь к ядру. можно и относительный.
     * @param kernelName - само название ядра. хранится внутри файла *.cl
     * @return скомпилированный и готовый к работе кернел
     */


private: /* gpu properties */
    cl_platform_id platform_id;
    cl_uint ret_num_platforms;
    cl_device_id device_id;
    cl_uint ret_num_devices;
    cl_context context;
    cl_command_queue command_queue;
    cl_queue_properties properties;

private: /* kernels */
    cl_kernel mergeSortStartKernel;
    cl_kernel mergeSortGlobalSmallKernel;
    cl_kernel mergeSortGlobalBigKernel;

private:
    cl_kernel compile_kernel(const char filename[], const char kernelName[]);

public:
    void initSortKernels();
public: /* getters */

    /* context */
    cl_context &getContext() { return context; }

    const cl_context &getContext() const { return context; }

    /* command_queue */
    cl_command_queue &getClCommandQueue() { return command_queue; }

    const cl_command_queue &getClCommandQueue() const { return command_queue; }

    /* mergeSortStartKernel */
    cl_kernel &getMergeSortStartKernel() { return mergeSortStartKernel; }

    const cl_kernel &getMergeSortStartKernel() const { return mergeSortStartKernel; }

    /* mergeSortGlobalSmallKernel */
    cl_kernel &getMergeSortGlobalSmallKernel() { return mergeSortGlobalSmallKernel; }

    const cl_kernel &getMergeSortGlobalSmallKernel() const { return mergeSortGlobalSmallKernel; }

    /* mergeSortGlobalBigKernel */
    cl_kernel &getMergeSortGlobalBigKernel() { return mergeSortGlobalBigKernel; }

    const cl_kernel &getMergeSortGlobalBigKernel() const { return mergeSortGlobalBigKernel; }

};


#endif //PRAO_COMPRESSER_GPUCONTEXT_H
