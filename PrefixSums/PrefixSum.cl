
void PrefixSums512(global int* data, global int* result, local int* cache, global int* tmpResultBuffer)
{
	//Store all data into the localMemory for faster access times.
	cache[ get_local_id(0) * 2 ] = data[ get_local_id(0) * 2 ];
	cache[ get_local_id(0) * 2 + 1 ] = data[ get_local_id(0) * 2 + 1 ];

	//Wait for all threads finished transfering data
	barrier(CLK_LOCAL_MEM_FENCE);

	//Up-Sweep
	for(int d = 0; d<9; ++d)
	{
		//Our Target Idx is 1,3,5,7,9....
		// then 3,7,11,15....
		//	tId is the thread Id, d iteration: ((tID + 1) * (2^d+1)) -1
		// which results in the upper numbers.
		// tId=0, d=0
		// e.g. ((0 + 1) * (2^(0+1))) -1 = (1) * (2) -1 = 2 - 1 = 1
		// tId=1, d=0
		// e.g. ((1 + 1) * (2^(0+1))) -1 = (2) * (2) -1 = 4 - 1 = 3
		// tId=1, d=1
		// e.g. ((1 + 1) * (2^(1+1))) -1 = (2) * (4) -1 = 8 - 1 = 7
		// d is iterated until 9 because log2(512) = 9
		int targetArrayIdx = ((get_local_id(0) + 1 ) * 1 << (d+1)) - 1;
		// Skip threads exceeding our limit (as we do need less and less threads per iteration)
		if( targetArrayIdx < 512 )
		{
			//The other index is 0,2,4,6,8....
			// then 4,8,12,16....
			// which leads to: targetIdx - (2^d)
			// tId=0, d=0
			// e.g. 1 - (2^0) = 1-1=0 
			// tId=1, d=0
			// e.g. 3 - (2^0) = 3-1=2 
			// tId=1, d=1
			// e.g. 7 - (2^1) = 7-2=5 
			int otherSrcIdx = targetArrayIdx - ( 1 << d );
			
			// Sum up both indexes and store it at the targetIdx
			cache[ targetArrayIdx ] = cache[ targetArrayIdx ] + cache[ otherSrcIdx ];
		}
		//Wait for all threads to complete this iteration step
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	//Once we're done one thread sets the last Element to zero
	if(get_local_id(0) == 0)
	{
		cache[ 511 ] = 0;
	}
	//And wait for that thread
	barrier(CLK_LOCAL_MEM_FENCE);

	//Down-Sweep
	for(int d = 9; d>=0; --d)
	{
		//The index calculation is the same as before but now in reverse,
		//so we start with one thread (d=9) and add more and more threads each iteration, by decreasing d
		int targetArrayIdx = ((get_local_id(0) + 1 ) * 1 << (d+1)) - 1;
		if( targetArrayIdx < 512 )
		{
			int otherSrcIdx = targetArrayIdx - ( 1 << d );
			// Temporarily save the values at the targetIdx
			int tmp = cache[ targetArrayIdx ];
			// Add the oldValue of the otherSrcIdx to the targetIdx
			cache[ targetArrayIdx ] += cache[ otherSrcIdx ];
			// Set the value of the otherSrcIdx to the old targetIdx's value.
			cache[ otherSrcIdx ] = tmp;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	//Special case, add the last Element to the cache in the recursion for the next group to be added.
	if( tmpResultBuffer != 0 && get_local_id(0) == 0)
	{
		tmpResultBuffer[ get_group_id( 0 ) ] = result[511];
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	//Write the cached values back to the global memory.
	result[ get_local_id(0) * 2 ] = cache[ get_local_id(0) * 2 ];
	result[ get_local_id(0) * 2 + 1 ] = cache[ get_local_id(0) * 2 +1];
}

kernel void PrefixSums(global int* data, global int* result, local int* cache, int size, global int* tmpResultBuffer)
{
	//Calculate the offset of the data according the current group id
	int offset = 512 * (get_group_id( 0 ));
	// Split several work groups to its own region
	PrefixSums512(data + offset, result + offset, cache,tmpResultBuffer);
	barrier( CLK_LOCAL_MEM_FENCE );
	//If we get a resultBuffer provided, because recursion is needed store our largest value from this group into the according cache Buffer
	//On which this kernel is run the the next recursion step
	if( tmpResultBuffer != 0 && get_local_id(0) == 0)
	{
		tmpResultBuffer[ get_group_id( 0 ) ] += result[(512* (get_group_id( 0 ))) +511 ];
	}


}