

kernel void MakeTile(global uchar4* inputTexture, global uchar4* outputTexture, uint width, uint height)
{
	unsigned int pixelIdx = (get_global_id(0));


	if(pixelIdx < width * height)
	{
		unsigned int pixelPosy = (get_global_id(0) / width);
		unsigned int pixelPosx = (get_global_id(0) % width);
		unsigned int pixelSamplePosx = ( ( get_global_id( 0 ) + ( width >> 1 ) ) % width );
		unsigned int pixelSamplePosy = ( ( ( get_global_id( 0 ) + ( (height / 2) * width ) ) / width ) % height );

		float originalImagePercentagex = (pixelPosx > (width / 2)) ?
		1.0f - ((((float)pixelPosx - ((float)width/2.0f)) / ((float)width/2.0f))) :
		((float)pixelPosx / ((float)width/2.0f));
		float sampleImagePercentagex = 1.0f - originalImagePercentagex;

		unsigned int sampleWidthPixelIdx = pixelPosy * width + pixelSamplePosx;
		unsigned int sampleHeightPixelIdx = (pixelSamplePosy) * width + pixelPosx;
		unsigned int sampleWidthHeightPixelIdx = (pixelSamplePosy) * width + pixelSamplePosx;

		uchar4 originalSample = convert_uchar4(((convert_float4(inputTexture[pixelIdx])) * originalImagePercentagex));
		originalSample += convert_uchar4( ( ( convert_float4( inputTexture[ sampleWidthPixelIdx ] ) ) * sampleImagePercentagex ) );


		float originalImagePercentagey = ( pixelPosy > ( height / 2 ) ) ?
			1.0f - ( ( ( ( float ) pixelPosy - ( ( float ) height / 2.0f ) ) / ( ( float ) height / 2.0f ) ) ) :
			( ( float ) pixelPosy / ( ( float ) height / 2.0f ) );
		float sampleImagePercentagey = 1.0f - originalImagePercentagey;


		uchar4 verticalSample = convert_uchar4( ( ( convert_float4( originalSample ) ) * originalImagePercentagey ) );
		uchar4 verticalSecondSample = convert_uchar4( ( ( convert_float4( inputTexture[ sampleHeightPixelIdx ] ) ) * originalImagePercentagex ) );
		verticalSecondSample += convert_uchar4( ( ( convert_float4( inputTexture[ sampleWidthHeightPixelIdx ] ) ) * sampleImagePercentagex ) );

		verticalSample += convert_uchar4( ( ( convert_float4( verticalSecondSample ) ) * sampleImagePercentagey ) );

		outputTexture[pixelIdx] = verticalSample;

	}



}