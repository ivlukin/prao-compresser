//
// Created by Dolgushev on 04.02.2019.
//

#include "testReader.h"
#include "../Calibration/CalibrationDataStorage.h"

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
    const char fileName[] = "../resources/heapSort.cl";
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


const size_t size_chunk_floats = 800;
const size_t global_workgroup = 33 * 48 * 4;
const size_t buffer_size = sizeof(cl_float) * global_workgroup * size_chunk_floats;
const size_t local_workgroup = 12;
char buff[0x1001];

float arr[size_chunk_floats * 4][global_workgroup / 4];

void start(){
    cl_int ret;
    cl_uint ret_num_platforms, ret_num_devices = 0;
    cl_platform_id platform_id[10];
    cl_device_id devices;

    cl_context context;
    cl_program program;

    char * kernel_source;
    size_t kernel_len;


    ret = clGetPlatformIDs(10, platform_id, &ret_num_platforms);
    if (ret_num_platforms != 2)
        throw logic_error("ret_num_platforms");
    ret = clGetDeviceIDs(platform_id[0], CL_DEVICE_TYPE_GPU, 1, &devices, &ret_num_devices);
    ret = clGetDeviceInfo(devices, CL_DEVICE_NAME, 0x100, buff, (size_t *)&ret);
    cout << buff << endl;
    context = clCreateContext(NULL, 1, &devices, NULL, NULL, &ret);
    queue = clCreateCommandQueue(context, devices, 0, &ret);

    readKernel(kernel_source, kernel_len);
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, &kernel_len, &ret);
    if (clBuildProgram(program, 1, &devices, NULL, NULL, NULL))
        throw logic_error("clBuildProgram");

    kernel = clCreateKernel(program, "heapSort", &ret);

    cl_in_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, 0, &ret);
    //cl_in2_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size*3, 0, &ret);
    //cl_out_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, 0, &ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_in_buff);
    //ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_in2_buff);
    //ret = clSetKernelArg(kernel, 1, size_chunk_floats * 2 * sizeof(cl_float), NULL);
    //ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_out_buff);
}

float fTimeStart, fTimeSum = 0;
//int tmp[buffer_size / 4];

void process_next(){
    cl_int ret;

    float fTimeStart = clock();

    ret = clEnqueueWriteBuffer(queue, cl_in_buff, CL_TRUE, 0, buffer_size, arr, 0, NULL, NULL);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_workgroup, &local_workgroup, 0, NULL, NULL);

    clEnqueueReadBuffer(queue, cl_in_buff, CL_TRUE, 0, buffer_size, arr, 0, NULL, NULL); // checking sorting was performed without errors
    for (int i = 0; i < 20275200 / 3200 ; ++i){
        for (int j = 0; j < 799; ++j){
            if (((float*)(arr))[i * 800 + j] < ((float*)(arr))[i * 800 + j + 1]) {
                cout << i << " " << j << endl;
                break;
            }
        }
    }
    fTimeSum += clock() - fTimeStart;

    return;
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
                reader->readNextPoints(arr[0], size_chunk_floats * 4);
                //testSort();
                process_next();
                ++i;
                cout << (i * 20275200) / 1024 << endl;
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
