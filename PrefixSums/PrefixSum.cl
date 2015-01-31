
void PrefixSums512(global int* data, global int* result, local int* cache, global int* tmpResultBuffer, int groupOffset)
{
	cache[ get_local_id(0) ] = data[ get_local_id(0) ];
	cache[ get_local_id(0) + 256 ] = data[ get_local_id(0) + 256 ];

	barrier(CLK_LOCAL_MEM_FENCE);

	for(int d = 0; d<9; ++d)
	{

		int targetArrayIdx = ((get_local_id(0) + 1 ) * 1 << (d+1)) - 1;
		if( targetArrayIdx < 512 )
		{
			int otherSrcIdx = targetArrayIdx - ( 1 << d );
			if( otherSrcIdx < 0 )
			{
				otherSrcIdx = 0;
			}
			cache[ targetArrayIdx ] = cache[ targetArrayIdx ] + cache[ otherSrcIdx ];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	if(get_local_id(0) == 0)
	{
		cache[ 511 ] = 0;
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	for(int d = 9; d>=0; --d)
	{

		int targetArrayIdx = ((get_local_id(0) + 1 ) * 1 << (d+1)) - 1;
		if( targetArrayIdx < 512 )
		{
			int otherSrcIdx = targetArrayIdx - ( 1 << d );

			int tmp = cache[ targetArrayIdx ];
			cache[ targetArrayIdx ] += cache[ otherSrcIdx ];
			cache[ otherSrcIdx ] = tmp;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	//Special case, add the last Element to the cache in the recursion for the next group to be added.
	if( tmpResultBuffer != 0 && get_local_id(0) == 0)
	{
		tmpResultBuffer[ get_group_id( 0 ) + groupOffset ] = result[511];
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	result[ get_local_id(0) ] = cache[ get_local_id(0) ];
	result[ get_local_id(0) + 256 ] = cache[ get_local_id(0) + 256 ];

	

	barrier(CLK_LOCAL_MEM_FENCE);
}

kernel void PrefixSums(global int* data, global int* result, local int* cache, int size, global int* tmpResultBuffer, int groupOffset)
{
	int offset = get_local_size( 0 ) * 2 * (get_group_id( 0 ) +groupOffset);
	int cacheOffset = get_local_size( 0 ) * 2 * (get_group_id( 0 ));
	// Split several work groups to its own region
	PrefixSums512(data + offset, result + offset, cache + cacheOffset,tmpResultBuffer,groupOffset);
	barrier( CLK_LOCAL_MEM_FENCE );
	if( tmpResultBuffer != 0 && get_local_id(0) == 0)
	{
		tmpResultBuffer[ get_group_id( 0 ) + groupOffset ] += result[(512* (get_group_id( 0 ) + groupOffset)) +511 ];
	}


}