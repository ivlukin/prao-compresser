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
    cl_kernel compile_kernel(const char filename[], const char kernelName[]);


private:
    cl_platform_id platform_id;
    cl_uint ret_num_platforms;
    cl_device_id device_id;
    cl_uint ret_num_devices;
    cl_context context;
    cl_command_queue command_queue;
    cl_queue_properties properties;

public: /* getters */

    /* context */
    cl_context &getContext() { return context; }

    const cl_context &getContext() const { return context; }

    /* command_queue */
    cl_command_queue &getClCommandQueue() { return command_queue; }

    const cl_command_queue &getClCommandQueue() const { return command_queue; }

};


#endif //PRAO_COMPRESSER_GPUCONTEXT_H
