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
    //const char fileName[] = "../resources/heapSort.cl";
    const char fileName[] = "../resources/nth_element.cl";
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


const int features_count = 9;
const size_t size_chunk_floats = 800;
const size_t global_workgroup = 33 * 48;
const size_t buffer_size = sizeof(cl_float) * global_workgroup * size_chunk_floats;
const size_t buffer_size_out = sizeof(cl_float) * global_workgroup * features_count;
const size_t local_workgroup = 12;
char buff[0x1001];

float arr_[global_workgroup * size_chunk_floats];
//float arr[size_chunk_floats * 4][global_workgroup / 4];
//float arr2[size_chunk_floats * 4][global_workgroup / 4];
float arr3[global_workgroup][size_chunk_floats];
float arr4[global_workgroup][features_count];

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
    ret = clGetDeviceIDs(platform_id[1], CL_DEVICE_TYPE_CPU, 1, &devices, &ret_num_devices);
    ret = clGetDeviceInfo(devices, CL_DEVICE_NAME, 0x100, buff, (size_t *)&ret);
    cout << buff << endl;
    context = clCreateContext(NULL, 1, &devices, NULL, NULL, &ret);
    queue = clCreateCommandQueue(context, devices, 0, &ret);

    readKernel(kernel_source, kernel_len);
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, &kernel_len, &ret);
    if (clBuildProgram(program, 1, &devices, NULL, NULL, NULL))
        throw logic_error("clBuildProgram");

    //kernel = clCreateKernel(program, "nth_element", &ret);
    kernel = clCreateKernel(program, "getMetrics", &ret);

    cl_in_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, 0, &ret);
    cl_in2_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size_out, 0, &ret);
    //cl_out_buff = clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, 0, &ret);
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_in_buff);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_in2_buff);
    //ret = clSetKernelArg(kernel, 1, size_chunk_floats * 2 * sizeof(cl_float), NULL);
    //ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_out_buff);
}

float fTimeStart, fTimeSum = 0;
//int tmp[buffer_size / 4];


int partition3_2(float *arr_, int L, int R){
    swap(arr_[(L + R) / 2], arr_[L]);
    float piv = arr_[L];
    while (true) {
        while (arr_[R] < piv) // && L < R
            --R;
        if (L < R)
            arr_[L++] = arr_[R];
        while (arr_[L] > piv && L < R)
            L++;
        if (L < R)
            arr_[R--] = arr_[L];
        else
            break;

    }
    arr_[L] = piv;
    return L;
}


float findOrderStatistic(float * array, int n, int k) {
    int left = 0, right = n - 1;
    while (true) {
        int mid = partition3_2(array, left, right);

        if (mid == k)
            return array[mid];
        else if (k < mid)
            right = mid - 1;
        else
            left = mid + 1;

    }
}

#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>


void process_next(){
    cl_int ret;

    float fTimeStart = clock();

    ret = clEnqueueWriteBuffer(queue, cl_in_buff, CL_TRUE, 0, buffer_size, arr3, 0, NULL, NULL);

    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_workgroup, &local_workgroup, 0, NULL, NULL);

    //clEnqueueReadBuffer(queue, cl_in_buff, CL_TRUE, 0, buffer_size, arr2, 0, NULL, NULL); // checking sorting was performed without errors
    clEnqueueReadBuffer(queue, cl_in2_buff, CL_TRUE, 0, buffer_size_out, arr4, 0, NULL, NULL); // checking sorting was performed without errors


    for(int i = 0; i < global_workgroup; ++i){
        float mn = *min_element(&arr3[i][0], &arr3[i][800]);
        float mx = *max_element(&arr3[i][0], &arr3[i][800]);
        int mx_i = (max_element(&arr3[i][0], &arr3[i][800]) - &arr3[i][0]);
        //double av = accumulate<float *, double>(&arr3[i][0], &arr3[i][800], 0) / 800.0;
        float med;
        med = findOrderStatistic(&arr3[i][0], 800, 400);
        nth_element(&arr3[i][0], &arr3[i][0] + 400, &arr3[i][800], greater<float>());
        med = arr3[i][400];

        double D = 0, av1 = 0, D2 = 0, av2 = 0;

        int count = 0;
        for (int j = 0; j < 800; ++j){
            av1 += arr3[i][j];
            if (arr3[i][j] > arr4[i][7] && arr3[i][j] < arr4[i][8]) {
                ++count;
                av2 += arr3[i][j];
            }
        }

        av1 /= 800;
        av2 /= count;
        for (int j = 0; j < 800; ++j){
            D += (arr3[i][j] - av1) * (arr3[i][j] - av1);
            if (arr3[i][j] > arr4[i][7] && arr3[i][j] < arr4[i][8])
                D2 += (arr3[i][j] - av2) * (arr3[i][j] - av2);
        }
        D /= 800;
        D2 /= count;

        if (mn != arr4[i][0] || mn == 0)
            cout << "mn " << i << endl;
        if (mx != arr4[i][1] || mx == 0)
            cout << "mx " << i << endl;
        if (mx_i != arr4[i][2])
            cout << "mx_i " << i << endl;
        if (fabs(av1 - arr4[i][3]) > 0.01)
            cout << "average " << i << endl;
        if (med != arr4[i][4] || med == 0)
            cout << "med " << i << endl;
        if (fabs(D - arr4[i][5]) > 0.1)
            cout << "D " << i << endl;
        if (fabs(D2 - arr4[i][6]) > 0.1)
            cout << "D2 " << i << endl;
    }
/*
    for (int i = 0; i < global_workgroup; ++i){
        float * a1 = (float *)arr3 + 800 * i;
        //float * a2 = (float *)arr2 + 800 * i;
        float * a2 = (float *)arr4 + features_count * i;


        float t15_1 = findOrderStatistic(a1, 800, 15);
        //nth_element(a1, a1 + 15, a1 + 800, std::greater<>());
        //float t15_1 = a1[15];
        //nth_element(a1, a1 + 559, a1 + 800, std::greater<>());
        float t559_1 = findOrderStatistic(a1, 800, 559);
        //float t559_1 = a1[559];
        float t287_1 = findOrderStatistic(a1, 800, 287);
        //nth_element(a1, a1 + 287, a1 + 800, std::greater<>());
        //float t287_1 = a1[287];

        float t15_2 = a2[0];
        float t559_2 = a2[1];
        float t287_2 = a2[2];
        if (t15_1 != t15_2 || t559_1 != t559_2 || t287_1 != t287_2 || t15_1 == 0 || t559_1 == 0 || t287_1 == 0)
            //if (t15_1 != t15_2 || t559_1 != t559_2)
            cout << i << endl;

//        for (int j = 0; j < 800; ++j) {
//            if (a1[i * 800 + j] != a2[i * 800 + j]) {
//                cout << i << " " << j << endl;
//                break;
//            }
//        }
//
//        for (int j = 0; j < 799; ++j){
//            if (((float*)(arr2))[i * 800 + j] < ((float*)(arr2))[i * 800 + j + 1]) {
//                cout << i << " " << j << endl;
//                break;
//            }
//        }

    }
    */
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
                //reader->readNextPoints(arr3[0], size_chunk_floats);
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
                        //arr3[j1][j2] = ((float *)(arr))[j1 + 33 * 48 * j2];
                        arr3[j1][j2] = arr_[j1 + global_workgroup * j2];
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
