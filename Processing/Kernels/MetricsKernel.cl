//Кернел для подсчета метрик

__kernel void
StandardDeviation(__global float *array, __global float *outDispersion, __global float *outMean, int arraySize) {

    __global float *curr = &array[get_global_id(0) * arraySize];

    /* calculating mean of exact array */
    __private float mean = 0;
    for (int i = 0; i < arraySize; ++i) {
        mean += curr[i];
    }
    mean /= arraySize;
    outMean[get_global_id(0)] = mean;

    /* calculating dispersion */
    /* это кстати несмещенная дисперсия */
    float dispersion = 0;
    for (int i = 0; i < arraySize; ++i) {
        float element = curr[i];
        dispersion += ((element - mean) * (element - mean));
    }
    dispersion /= (float) (arraySize - 1);
    outDispersion[get_global_id(0)] = dispersion;
}