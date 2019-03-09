//
// Created by Dolgushev on 04.02.2019.
//

#include "testReader.h"
#include "../Calibration/CalibrationDataStorage.h"
#include "../Metrics/MetricsType.h"

#include <CL/cl.h>



CalibrationDataStorage * readCalibrationDataStorage(string path_calibration){
    float start = 0, diff = 0;
    auto *storage = new CalibrationDataStorage();

    start = clock();
    storage->add_items_from_file(path_calibration);
    diff = (clock() - start) / CLOCKS_PER_SEC;
    cout << "reading calibration file took " << diff << " sec" << endl;

    return storage;
}


char* readKernel(char *&source_str, size_t &source_size){
    size_t MAX_SOURCE_SIZE = 1000000;
    FILE *fp;
    //const char fileName[] = "../resources/heapSort.cl";
    const char fileName[] = "../resources/nth_element_CPU_edition.cl";
    int i;

    try {
        fp = fopen(fileName, "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        source_str = (char *)malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
    }
    catch (int a) {
        printf("%f", a);
    }
    return source_str;
}

cl_command_queue queue;
cl_kernel kernel;
cl_mem cl_in_buff, cl_in2_buff, cl_out_buff, cl_cache_buff;


const float left_percentile = 0.02f;
const float right_percentile = 1.0f - 0.3f;

const size_t size_chunk_floats = 800;
const size_t global_workgroup = 33 * 48;
const size_t local_workgroup = 12;

const size_t buffer_size = sizeof(cl_float) * global_workgroup * size_chunk_floats;
const size_t buffer_size_out = sizeof(metrics) * global_workgroup;


void start(){
    cl_int ret;
    cl_uint ret_num_platforms, ret_num_devices = 0;
    cl_platform_id platform_id[10];
    cl_device_id devices;
    char buff[0x1001];

    cl_context context;
    cl_program program;

    char * kernel_source;
    size_t kernel_len;


    ret = clGetPlatformIDs(10, platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id[1], CL_DEVICE_TYPE_CPU, 1, &devices, &ret_num_devices);
    ret = clGetDeviceInfo(devices, CL_DEVICE_NAME, 0x100, buff, (size_t *)&ret);
    cout << buff << endl;
    context = clCreateContext(NULL, 1, &devices, NULL, NULL, &ret);
    queue = clCreateCommandQueue(context, devices, 0, &ret);

    readKernel(kernel_source, kernel_len);
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, &kernel_len, &ret);
    ret = clBuildProgram(program, 1, &devices, NULL, NULL, NULL);
    if (ret == -11) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, devices, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, devices, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        cout << log << endl;
        exit(-1);
    }

    //kernel = clCreateKernel(program, "nth_element", &ret);
    kernel = clCreateKernel(program, "getMetrics", &ret);

    cl_in_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, 0, &ret);
    cl_in2_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size_out, 0, &ret);
    //cl_out_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, 0, &ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_in_buff);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_in2_buff);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_int), &size_chunk_floats);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_float), &left_percentile);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_float), &right_percentile);
    //ret = clSetKernelArg(kernel, 1, size_chunk_floats * 2 * sizeof(cl_float), NULL);
    //ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_out_buff);
}

float fTimeStart, fTimeSum = 0;


float arr_[global_workgroup * size_chunk_floats];
float data_reordered_buffer[global_workgroup][size_chunk_floats];
metrics metrics_buffer[global_workgroup];

void process_next(){
    cl_int ret;

    float fTimeStart = clock();

    ret = clEnqueueWriteBuffer(queue, cl_in_buff, CL_TRUE, 0, buffer_size, data_reordered_buffer, 0, NULL, NULL);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_workgroup, &local_workgroup, 0, NULL, NULL);

    ret = clEnqueueReadBuffer(queue, cl_in2_buff, CL_TRUE, 0, buffer_size_out, metrics_buffer, 0, NULL, NULL); // checking sorting was performed without errors


    metrics::check_natively_over_CPU(data_reordered_buffer[0], metrics_buffer, global_workgroup, size_chunk_floats, left_percentile, right_percentile);

    fTimeSum += clock() - fTimeStart;
}


void testReader(){
    start();
    //string path_calibration = "../resources/smallCalibrationParts1.txt";
    string path_calibration = "../resources/bigCalibrationParts.txt";
    CalibrationDataStorage *storage = readCalibrationDataStorage(path_calibration);

    //ifstream in{R"(../resources/filesListSmall.txt)"};
    ifstream in{R"(../resources/filesListBig.txt)"};
    while (!in.eof()){
        FilesListItem item;
        in >> item;
        DataReader * reader = item.getDataReader(10);
        reader->setCalibrationData(storage);

        float start = 0, diff = 0;
        start = clock();

        int i = 0;
        try {
            while(!reader->eof()) {
                //reader->readNextPoints(data_buffer[0], size_chunk_floats);
                reader->readNextPoints(arr_, size_chunk_floats);
                int count = 0;
                for (int j = 0; j < size_chunk_floats * global_workgroup; ++j)
                    count += ((float*)(arr_))[j] == 0;
                if (count != 0){
                    cout << "found null: " << count << endl;
                    count = 0;
                }

                for (int j1 = 0; j1 < global_workgroup; ++j1){
                    for (int j2 = 0; j2 < size_chunk_floats; ++j2){
                        //data_buffer[j1][j2] = ((float *)(arr))[j1 + 33 * 48 * j2];
                        data_reordered_buffer[j1][j2] = arr_[j1 + global_workgroup * j2];
                    }
                }
                //testSort();
                process_next();
                ++i;
                if (i % 4 == 0)
                    cout << (i * 5068800) / 1024 << endl;
            }
        }
        catch (logic_error e){

        }

        diff = (clock() - start) / CLOCKS_PER_SEC;

        cout << "calibration took " << diff << " sec" << endl;

        diff = (clock() - start) / CLOCKS_PER_SEC;

        cout << "GPU took " << fTimeSum  / CLOCKS_PER_SEC << " sec" << endl;
    }
}
