
kernel void sum(global int* in, global int* count, global int* tmpSums, global int* out)
{ 
	int threadIdx = get_global_id(0);
	int localCount = *count;
	tmpSums[threadIdx] = in[threadIdx*2]+in[threadIdx*2+1];
	localCount /=2;
	barrier(CLK_GLOBAL_MEM_FENCE);
	
	do 
	{
		if((threadIdx & 1) != 1)
		{
			threadIdx /=2;
			tmpSums[threadIdx] = tmpSums[threadIdx*2]+tmpSums[threadIdx*2+1];
		}
		localCount /=2;
		barrier(CLK_GLOBAL_MEM_FENCE);
	} while(localCount > 1);
	
	barrier(CLK_GLOBAL_MEM_FENCE);
	if(get_global_id(0)==0)
	{
		*out = tmpSums[0];
	}

}
//Benchmarking turns out, this is slightly faster than the one above
kernel void sum2(global int* in, global int* count, global int* tmpSums, global int* out)
{ 
	int threadIdx = get_global_id(0);
	int localCount = *count;
	tmpSums[threadIdx] = in[(threadIdx<<1)]+in[(threadIdx<<1)+1];
	localCount >>=1;
	barrier(CLK_GLOBAL_MEM_FENCE);
	
	do 
	{
		if((threadIdx & 1) != 1)
		{
			threadIdx >>=1;
			tmpSums[threadIdx] = tmpSums[threadIdx<<1]+tmpSums[(threadIdx<<1)+1];
		}
		localCount >>=1;
	barrier(CLK_GLOBAL_MEM_FENCE);
	} while(localCount > 1);
	
	barrier(CLK_GLOBAL_MEM_FENCE);

	if(get_global_id(0)==0)
	{
		*out = tmpSums[0];
	}
}

