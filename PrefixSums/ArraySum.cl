

kernel void ArraySum512(global int* src, global int* cachedSums)
{
	src[ get_local_id(0) * 2 ] = src[ get_local_id(0) * 2 ] + cachedSums[ get_group_id(0) ];
	src[ get_local_id(0) * 2 + 1 ] = src[ get_local_id(0) * 2 + 1 ] + cachedSums[ get_group_id(0) ];
}