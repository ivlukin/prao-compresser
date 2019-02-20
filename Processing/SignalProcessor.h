//
// Created by sorrow on 18.02.19.
//
/* this is opencl sandbox */
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else

#include <CL/cl.h>

#endif

#ifndef PRAO_COMPRESSER_SIGNALPROCESSOR_H
#define PRAO_COMPRESSER_SIGNALPROCESSOR_H


class SignalProcessor {
public:
    SignalProcessor();
    cl_kernel compile_kernel(const char filename[]);

private:
    cl_platform_id platform_id;
    cl_uint ret_num_platforms;
    cl_device_id device_id;
    cl_uint ret_num_devices;
    cl_context context;
    cl_command_queue command_queue;
    cl_queue_properties properties;


};


#endif //PRAO_COMPRESSER_SIGNALPROCESSOR_H
