
inline void swap(__global float *a, __global float *b) {
	float tmp;
	tmp = *b;
	*b = *a;
	*a = tmp;
}

void siftDown(int i, int heap_size, __global float* curr, int dir){
    while (2 * i + 1 < heap_size) { 
        int left = 2 * i + 1;            
        int right = 2 * i + 2;    
        if (right < heap_size && curr[right] < curr[left] == dir)
            left = right;
        if (curr[i] <= curr[left] == dir)
            break;
        swap(&curr[i], &curr[left]);
        i = left;
    }
}

__kernel void jSort(__global float* inp){
    const int array_size = 800;
    int heap_size = 800;
	__global float* curr = &inp[get_global_id(0) * array_size];
	
    
    for (int i = heap_size / 2; i >= 0; --i)
        siftDown(i, heap_size, curr, 1);
    for (int i = 0; i < heap_size / 2; ++i)
        swap(&curr[i], &curr[heap_size - i - 1]);
    for (int i = heap_size / 2; i >= 0; --i)
        siftDown(i, heap_size, curr, 0);
	
	for(int i = 1 ; i < array_size ; ++i)
    {
		int j;
        float value = curr[i];
        for (j = i - 1; j >= 0 && curr[j] < value; --j)
            curr[j + 1] = curr[j];
        curr[j + 1] = value;
    }
}
