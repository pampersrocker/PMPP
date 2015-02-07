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
			//brightness = ( convert_int(curPixel.x * 0.299f) +
			//	convert_int(curPixel.y * 0.587f) +
			//	convert_int(curPixel.z * 0.114f));
			//brightness = curPixel.x + curPixel.y + curPixel.z;
			//brightness/=3;
			brightness = (int)round(curPixel.x * 0.299f + curPixel.y * 0.587f + curPixel.z * 0.114f);
			//brightness = curPixel.w;
			atomic_inc( histogram + brightness );
		}
	}
}