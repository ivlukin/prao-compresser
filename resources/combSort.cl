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


__kernel void combSort(__global int* inp) 
{ 
	int size = 800;
	__global int* curr = &inp[get_global_id(0) * size];
    int gap = size; 
	int tmp;
  
    bool swapped = true; 
  
    while (gap != 1 || swapped == true) 
    { 
        gap = (gap*10)/13;
		if (gap < 1) 
			gap = 1;
  
        swapped = false; 
  
        for (int i=0; i < size - gap; i++) 
        { 
            if (curr[i] < curr[i+gap]) 
            { 
				swap(&curr[i], &curr[i+gap]);
                swapped = true; 
            } 
        } 
    } 
}

__kernel void combSort2(__global float* inp) {
	int size = 800;
	const float shrink = 1.3f;
	float swap;
	int i, gap = size;
	bool swapped = false;
		
	__global float* curr = &inp[get_global_id(0) * size];
		
	
	while ((gap > 1) || swapped) {
		if (gap > 1) {
			gap = (int)((float)gap / shrink);
		}

		swapped = false;

		for (i = 0; gap + i < size; ++i) {
			if (curr[i] < curr[i + gap]) {
				swap = curr[i];
				curr[i] = curr[i + gap];
				curr[i + gap] = swap;
				swapped = true;
			}
		}
	}
	
}