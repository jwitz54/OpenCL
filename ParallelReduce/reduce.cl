//#include <stdio.h>
//#include <CL/cl.h>

__kernel
void reduce(__global int *input,
	    	__global int *output,
	                 int numData,
	                 int groupSize)
{
	int gid = get_global_id(0);
	int gsize = get_global_size(0);
	
	int lid = get_local_id(0);
	int lsize = get_local_size(0);

	__local int lmem[4];
	lmem[lid] = input[gid];

	//printf("thread: %i data: %i\n", gid, input[gid]);

	barrier(CLK_LOCAL_MEM_FENCE); 

	for (int s = lsize/2;
	     s > 0;
	     s >>= 1){	
		if(lid < s){
			//printf("lmem before: %i\n", lmem[lid]);
			lmem[lid] += lmem[lid + s];
			//printf("lmem after: %i\n", lmem[lid]);
		}		
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	
	int sum = 0;
	if (lid == 0){
		printf("thread: %i ldata: %i\n", gid, lmem[lid]);
		sum += lmem[0];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	if (gid == 0){
		output[gid] = sum;
	}

	// if (gid == 0){
	// 	for (int i = 0; i < 20; i++){
	// 		printf ("%i ", output[i]);
	// 	}
	// }
}
