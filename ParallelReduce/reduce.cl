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

	__local int lmem[5];
	lmem[lid] = input[gid];

	barrier(CLK_LOCAL_MEM_FENCE); 

	for (int s = lsize/2;
	     s > lsize;
	     s >>= 1){	
		if(lid < s){
			lmem[lid] += lmem[lid + s];
		}
		barrier(CLK_LOCAL_MEM_FENCE); 	
	}
	
	if (lid == 0){
		output[gid] = lmem[0];
	}
}
