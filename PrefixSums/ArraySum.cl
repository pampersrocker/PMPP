

kernel void ArraySum512(global int* src, global int* cachedSums, int globalSize)
{
	int groupId = get_group_id( 0 );
	int globalId = get_global_id(0);
	if( ((globalId * 2)) < globalSize )
	{
		src[ ((globalId * 2)) ] = src[ ((globalId * 2)) ] + cachedSums[ groupId ];
		if( ((globalId * 2+1)) < globalSize )
		{
			src[ ((globalId * 2+1)) ] = src[ ((globalId * 2+1)) ] + cachedSums[ groupId ];
		}
	}
}