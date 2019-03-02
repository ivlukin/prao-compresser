

__kernel void qSort(__global float * inp) {
	const int elements = 800;
	const int MAX_LEVELS = 30;
	__local int local_stack[2 * 30 * 12];
	__global float* arr = &inp[get_global_id(0) * elements];
	
	__local int* beg = &local_stack[get_local_id(0) * MAX_LEVELS * 2];
	__local int* end = &local_stack[get_local_id(0) * MAX_LEVELS * 2 + MAX_LEVELS];


	float piv;
	int i = 0, L, R, swap;

	beg[0] = 0;
	end[0] = elements;
	while (i >= 0) {
		L = beg[i]; 
		R = end[i] - 1;
		if (L < R) {
            piv = arr[L];
            while (L < R) {
                while (arr[R] < piv && L < R) R--;
                if (L < R) arr[L++] = arr[R];
                while (arr[L] > piv && L < R) L++;
                if (L < R) arr[R--] = arr[L];
            }
            arr[L] = piv;
            beg[i + 1] = L + 1;
            end[i + 1] = end[i];
            end[i++] = L;
            if (end[i] - beg[i] > end[i - 1] - beg[i - 1]) {
                swap = beg[i];
                beg[i] = beg[i - 1];
                beg[i - 1] = swap;
                swap = end[i];
                end[i] = end[i - 1];
                end[i - 1] = swap;
            }
        }
		else i--; 
	}
	return; 
}