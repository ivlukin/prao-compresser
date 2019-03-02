/*
good working heapsort. Local size - 8-16: 12
const size_t size_chunk_floats = 800;
const size_t global_workgroup = 33 * 48 * 4;
const size_t buffer_size = sizeof(cl_float) * global_workgroup * size_chunk_floats;
const size_t local_workgroup = 12;
*/

inline void swap(__global float *a, __global float *b) {
	float tmp;
	tmp = *b;
	*b = *a;
	*a = tmp;
}

void siftDown(int i, int heap_size, __global float* curr){
    while (2 * i + 1 < heap_size) { 
        int left = 2 * i + 1;            
        int right = 2 * i + 2;    
        if (right < heap_size && curr[right] < curr[left])
            left = right;
        if (curr[i] <= curr[left])
            break;
        swap(&curr[i], &curr[left]);
        i = left;
    }
}


__kernel void heapSort(__global int* inp){
    const int array_size = 800;
    int heap_size = 800;
	__global int* curr = &inp[get_global_id(0) * array_size];
	
    for (int i = heap_size / 2; i >= 0; --i)
        siftDown(i, heap_size, curr);
    
    for (int i = 0; i < array_size - 1; ++i){
        swap(&curr[0], &curr[heap_size - 1]);
        --heap_size;
        siftDown(0, heap_size, curr);
    }
}