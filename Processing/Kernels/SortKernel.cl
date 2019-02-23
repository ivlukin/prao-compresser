__kernel void sort(__global float *arr) {
    // получаем текущий id.
    int gid = get_global_id(0);

    // TODO: получить все параметры нужные для BitonicSort

    inline void
    BitonicSort(__local float *arr, __local float *arrHelper, const unsigned int pairCount, __private int localId) {
        #pragma unroll
        for (int globalGran = 2; globalGran <= 1024; globalGran << = 1) {
            for (int localGran = globalGran; localGran >= 2; localGran >>= 1) {
                if (localId < pairCount) {
                    int halfLocalGran = localGran >> 1;
                    int arrId = localId % halfLocalGran + (localId / halfLocalGran) * localGran;
                    int arrPairId = arrId + halfLocalGran;
                    bool dir = ((arrId / globalGran) % 2) == 0; /* true - asc, false - desc */
                    compare(&arr[arrId], &arr[arrPairId], &arrHelper[arrId], &arrHelper[arrPairId], dir);
                }
                barrier(CLK_LOCAL_MEM_FENCE);
            }
        }
    }


}