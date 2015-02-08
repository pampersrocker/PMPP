#define LX get_local_id(0)
#define GID get_group_id(0)
#define LSIZE get_local_size(0)
#define GX get_global_id(0)

kernel void calcStatisticAtomic(global uchar4* data, int size, global int* histogram, int pixelperThread)
{
	uchar brightness = 0;
	uchar4 curPixel;

	for(int pixelIdx = 0; pixelIdx<pixelperThread; ++pixelIdx)
	{
		int dataIdx = GID * pixelperThread * LSIZE + LX + pixelIdx * LSIZE;
		if( dataIdx < size)
		{
			curPixel = data[dataIdx];
			brightness = (int)round(curPixel.x * 0.299f + curPixel.y * 0.587f + curPixel.z * 0.114f);
			atomic_inc( histogram + brightness );
		}
	}
}

kernel void calcStatisticAtomicRGB(global uchar4* data, int size, global int* histogram, int pixelperThread)
{
	uchar brightness = 0;
	uchar4 curPixel;

	for(int pixelIdx = 0; pixelIdx<pixelperThread; ++pixelIdx)
	{
		int dataIdx = GID * pixelperThread * LSIZE + LX + pixelIdx * LSIZE;
		if( dataIdx < size)
		{
			curPixel = data[dataIdx];
			brightness = curPixel.x;
			atomic_inc( histogram + 256*0 + brightness );
			brightness = curPixel.y;
			atomic_inc( histogram + 256*1 + brightness );
			brightness = curPixel.z;
			atomic_inc( histogram + 256*2 + brightness );
		}
	}
}