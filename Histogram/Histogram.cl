
#define LX get_local_id(0)
#define GID get_group_id(0)
#define LSIZE get_local_size(0)
#define GX get_global_id(0)

kernel void calcStatistic(global uchar4* data, int size, global int* histogram)
{
	local tmpHistogram[32][256];

	for(int histogramIdx = 0; histogramIdx < 256; ++histogramIdx)
	{
		tmpHistogram[LX][histogramIdx] = 0;
	}

	// No sync needed, because every thread sets its own data, so no thread needs to wait for the others

	int brightness = 0;
	uchar4 curPixel;

	for(int pixelIdx = 0; pixelIdx<256; ++pixelIdx)
	{
		int dataIdx = GID * 256 * LSIZE + LX + pixelIdx * 32;
		if( dataIdx < size)
		{
			curPixel = data[dataIdx];
			brightness = (int)(curPixel.x * 0.299f + curPixel.y * 0.587f + curPixel.z * 0.114f);
			++tmpHistogram[LX][brightness];
		}
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	for(int i = 1; i < 8; ++i)
	{
		tmpHistogram[0][LX * i] += tmpHistogram[LX][LX + i * LSIZE];
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	for(int i = 0; i < 8; ++i)
	{
		histogram[GID *256 + LX + i * LSIZE] = tmpHistogram[0][LX + i * LSIZE];
	}

}