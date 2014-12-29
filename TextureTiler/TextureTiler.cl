

kernel void MakeTile(global uchar4* inputTexture, global uchar4* outputTexture, uint width, uint height)
{
	unsigned int pixelIdx = (get_global_id(0));


	if(pixelIdx < width * height)
	{
		unsigned int pixelPosy = (get_global_id(0) / width);
		unsigned int pixelPosx = (get_global_id(0) % width);
		unsigned int pixelSamplePosx = ((get_global_id(0) + (width >>1)) % width);

		float originalImagePercentage = (pixelPosx > (width / 2)) ?
		1.0f - ((((float)pixelPosx - ((float)width/2.0f)) / ((float)width/2.0f))) :
		((float)pixelPosx / ((float)width/2.0f));
		float sampleImagePercentage = 1.0f - originalImagePercentage;

		unsigned int sampleWidthPixelIdx = pixelPosy * width + pixelSamplePosx;

		uchar4 output = convert_uchar4(((convert_float4(inputTexture[pixelIdx])) * originalImagePercentage));
		output += convert_uchar4(((convert_float4(inputTexture[sampleWidthPixelIdx])) * sampleImagePercentage));

		outputTexture[pixelIdx] = output;

	}



}