
void PrefixSums512(global int* data, global int* result)
{
	local int[512] cache;
	cache[ get_local_id(0) ] = data[ get_local_id(0) ];
	cache[ get_local_id(0) + 256 ] = data[ get_local_id(0) + 256 ];

	barrier(CLK_LOCAL_MEM_FENCE);

	for(int d = 0; d<9; ++d)
	{

		int targetArrayIdx = ((get_local_id(0) + 1 ) * 2 << d) - 1;
		if( targetArrayIdx < 512 )
		{
			int otherSrcIdx = targetArrayIdx - ( 2 << d );
			cache[ i ] = cache[ targetArrayIdx ] + cache[ otherSrcIdx ];
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

		int targetArrayIdx = ((get_local_id(0) + 1 ) * 2 << d) - 1;
		if( targetArrayIdx < 512 )
		{
			int otherSrcIdx = targetArrayIdx - ( 2 << d );
			int tmp = cache[ targetArrayIdx ];
			cache[ i ] += cache[ otherSrcIdx ];
			cache[ otherSrcIdx ] = tmp;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	result[ get_local_id(0) ] = cache[ get_local_id(0) ];
	result[ get_local_id(0) + 256 ] = cache[ get_local_id(0) + 256 ];

	barrier(CLK_LOCAL_MEM_FENCE);
}

kernel void PrefixSums(global int* data, global int* result, global int size)
{
	PrefixSums512(data, result);
}