
__kernel void sum(__global int* in, __global int* count, __global int* tmpSums, __global int* out)
{ 
	int threadIdx = get_global_id(0);
	int localCount = *count;
	tmpSums[threadIdx] = in[threadIdx*2]+in[threadIdx*2+1];
	localCount /=2;
	barrier(CLK_LOCAL_MEM_FENCE);
	
	do 
	{
		if((threadIdx & 1) != 1)
		{
			threadIdx /=2;
			tmpSums[threadIdx] = tmpSums[threadIdx*2]+tmpSums[threadIdx*2+1];
		}
		localCount /=2;
		barrier(CLK_LOCAL_MEM_FENCE);
	} while(localCount > 1);
	
	barrier(CLK_LOCAL_MEM_FENCE);
	if(get_global_id(0))
	{
		*out = tmpSums[0];
	}

}
