
inline void compare(
        __local float *A,
        __local float *B,
        __local float *idA,
        __local float *idB,
        bool dir) {
    if ((*A > *B) == dir) {
//#pragma HLS PIPELINE
        float t;
        t = *A;
        *A = *B;
        *B = t;
        t = *idA;
        *idA = *idB;
        *idB = t;
    }
}

__kernel void MergeSort(__local float *arr,
                        __local float *arrHelper,
                        const unsigned int pairCount,
                        __private int localId) {
#pragma unroll
    for (int globalGran = 2; globalGran <= 1024; globalGran <<= 1) {
        for (int localGran = globalGran; localGran >= 2; localGran >>= 1) {
            if (localId < pairCount) {
                int halfLocalGran = localGran >> 1;
                int arrId = localId % halfLocalGran + (localId / halfLocalGran) * localGran;
                int arrPairId = arrId + halfLocalGran;
                bool dir = ((arrId / globalGran) % 2) == 0; // true = asc, false = desc
                compare(&arr[arrId], &arr[arrPairId], &arrHelper[arrId], &arrHelper[arrPairId], dir);
            }
            barrier(CLK_LOCAL_MEM_FENCE);
        }


    }
}

