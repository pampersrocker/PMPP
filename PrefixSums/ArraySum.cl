

kernel void ArraySum512(global int* src, global int* cachedSums, int globalSize, int groupOffset)
{
	int groupId = get_group_id( 0 ) + groupOffset;
	int globalId = get_global_id(0);
	int elementOffset =groupOffset * 512;
	if( ((globalId * 2)+ elementOffset) < globalSize )
	{
		src[ ((globalId * 2)+ elementOffset) ] = src[ ((globalId * 2)+ elementOffset) ] + cachedSums[ groupId ];
		if( ((globalId * 2+1)+ elementOffset) < globalSize )
		{
			src[ ((globalId * 2+1)+ elementOffset) ] = src[ ((globalId * 2+1)+ elementOffset) ] + cachedSums[ groupId ];
		}
	}
}