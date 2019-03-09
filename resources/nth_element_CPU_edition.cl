
inline void swap(__global float *a, __global float *b) {
	float tmp;
	tmp = *b;
	*b = *a;
	*a = tmp;
}


int partition(__global float *curr, int L, int R){
    swap(&curr[(L + R) / 2], &curr[L]);
    float piv = curr[L];
    while (true) {
        while (curr[R] < piv) // && L < R
            --R;
        if (L < R)
            curr[L++] = curr[R];
        while (curr[L] > piv && L < R)
            L++;
        if (L < R)
            curr[R--] = curr[L];
        else
            break;

    }
    curr[L] = piv;
    return L;
}

// current array with size of N (right). K-th element will be returned
float nth_element_internal(__global float * curr, int right, int k) {
	--right;
    int left = 0;
    while (true) {
        int mid = partition(curr, left, right);
        if (mid == k) {
            return curr[mid];
		}
        else if (k < mid) 
            right = mid - 1;
        else 
            left = mid + 1;
    }
}





__kernel void getMetrics(__global float* inp, __global float* output){
    const int array_size = 800;
    const int features_count = 9;
	const int right = array_size * 0.02;
	const int left = array_size * 0.7;
	
	__global float* curr_in = &inp[get_global_id(0) * array_size];
	__global float* curr_out = &output[get_global_id(0) * features_count];
	
	
	float min = 1000000;
	float max = -100;
	int min_i;
	int max_i;
	
	for (int i = 0; i < array_size; ++i){
		float curr_val = curr_in[i];
		if (curr_val < min) {
			min = curr_in[i];
			min_i = i;
		}
		if (curr_val > max){
			max = curr_in[i];
			max_i = i;
		}
	}
	
	
	float left_bound = nth_element_internal(curr_in, array_size, left); 
	float right_bound = nth_element_internal(curr_in, array_size, right);
	float med_value = nth_element_internal(curr_in, array_size, array_size / 2);
	
	
	double av = 0;
	double av_sqt = 0;
	double av_bounded = 0;
	double av_sqt_bounded = 0;
	
	int count = 0;
	for (int i = 0; i < array_size; ++i){
		float curr_val = curr_in[i];
		
		if (curr_val > left_bound && curr_val < right_bound){
			av_bounded += curr_val;
			av_sqt_bounded += curr_val * curr_val;
			++count;
		}
		
		av += curr_val;
		av_sqt += curr_val * curr_val;
	}
	
	av /= array_size;
	av_sqt /= array_size;
	av_bounded /= count;
	av_sqt_bounded /= count;
	
	double D = (av_sqt - av * av);
	double D_bounded = (av_sqt_bounded - av_bounded * av_bounded);
	
	curr_out[0] = (float)min; // minimum of all
	curr_out[1] = (float)max; // maximum of all
	curr_out[2] = (float)max_i; // index of maximum of all
	curr_out[3] = (float)av;// / array_size; // average of all
	curr_out[4] = med_value; // median of all
	curr_out[5] = (float)D; // variance of all
	curr_out[6] = (float)D_bounded; // variance of bounded
	curr_out[7] = (float)left_bound; // variance of all
	curr_out[8] = (float)right_bound; // variance of bounded
	//curr_out[] = ; // 
}
