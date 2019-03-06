//Кернел для подсчета метрик

__kernel void StandardDeviation(__global float *array, __global float *out, int arraySize) {
    __global float *curr = &array[get_global_id(0) * arraySize];

    /* calculating mean of exact array */
    __private float mean = 0;
    for (int i = 0; i < arraySize; ++i) {
        mean += curr[i];
    }
    mean /= arraySize;

    /* calculating dispersion */
    /* это кстати несмещенная дисперсия */
    float dispersion = 0;
    for (int i = 0; i < arraySize; ++i) {
        dispersion += ((curr[i] - mean) * (curr[i] - mean));
    }
    dispersion /= (float)(arraySize - 1);
    out[get_global_id(0)] = dispersion;
}