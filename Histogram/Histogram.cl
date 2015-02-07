
#define LX get_local_id(0)
#define GID get_group_id(0)
#define LSIZE get_local_size(0)
#define GX get_global_id(0)

kernel void calcStatistic(global uchar4* data, int size, global int* histogram, int pixelperThread)
{
	local tmpHistogram[32][256];

	for(int histogramIdx = 0; histogramIdx < 256; ++histogramIdx)
	{
		tmpHistogram[LX][histogramIdx] = 0;
	}

	// No sync needed, because every thread sets its own data, so no thread needs to wait for the others

	uchar brightness = 0;
	uchar4 curPixel;

	for(int pixelIdx = 0; pixelIdx<pixelperThread; ++pixelIdx)
	{
		int dataIdx = GID * pixelperThread * LSIZE +
						LX +
						pixelIdx * LSIZE;
		if( dataIdx < size)
		{
			curPixel = data[dataIdx];
			brightness = (int)round(curPixel.x * 0.299f + curPixel.y * 0.587f + curPixel.z * 0.114f);
			++tmpHistogram[LX][brightness];
		}
	}

	barrier(CLK_GLOBAL_MEM_FENCE);
	// Add all except the first to the first work item cache
	for(int k = 1; k < LSIZE; ++k)
	{
		for(int i = 0; i < 8; ++i)
		{
			tmpHistogram[0][LX + i * LSIZE] += tmpHistogram[k][LX + i * LSIZE];
		}
	}

	barrier(CLK_GLOBAL_MEM_FENCE);

	for(int i = 0; i < 8; ++i)
	{
		histogram[GID *256 + LX + i * LSIZE] = tmpHistogram[0][LX + i * LSIZE];
	}

}