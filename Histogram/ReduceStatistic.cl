
#define LX get_local_id(0)
#define GID get_group_id(0)
#define LSIZE get_local_size(0)
#define GX get_global_id(0)

kernel void reduceStatistic(global int* histogram, int groupCount)
{
	for( int i = 1; i < groupCount; ++i)
	{
		histogram[ LX ] += histogram[ LX + i * 256 ];
	}

}