

kernel void ArraySum512(global int* src, global int* cachedSums, int globalSize)
{
	if( get_global_id( 0 ) < globalSize )
	{
		src[ get_global_id( 0 ) * 2 ] = src[ get_global_id( 0 ) * 2 ] + cachedSums[ get_group_id( 0 ) ];
		src[ get_global_id( 0 ) * 2 + 1 ] = src[ get_global_id( 0 ) * 2 + 1 ] + cachedSums[ get_group_id( 0 ) ];
	}
}