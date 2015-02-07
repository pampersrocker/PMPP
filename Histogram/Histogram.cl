
#define LX get_local_id(0)
#define GID get_group_id(0)
#define GX get_global_id(0)

kernel void calcStatistic(global uchar4* data, int size, global int* histogram)
{
	local tmpHistogram[32][256];

	for(int histogramIdx = 0; histogramIdx < 256; ++histogramIdx)
	{
		tmpHistogram[LX][histogramIdx] = 0;
	}

	// No sync needed, because every thread sets its own data, so no thread needs to wait for the others

	

}