// TextureTiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "OpenCL.h"

typedef OpenCLProgram_tpl<1> OpenCLProgram;


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window( sf::VideoMode( 1280, 900), "Texture Tiler" );

	sf::Texture gravel;
	sf::Texture gravelTiled;
	sf::Sprite rawSprite;

	sf::Image tiledImage;
	if( !gravel.loadFromFile( "Gravel.jpg" ) )
	{
		std::cout << "Failed to load Gravel.jpg!" << std::endl;
	}
	sf::Image rawImage = gravel.copyToImage();

	tiledImage.create( rawImage.getSize().x, rawImage.getSize().y );

	float scale = 1.0f;

	size_t size = rawImage.getSize().x * rawImage.getSize().y;

	cl_uchar4* imageBuffer = new cl_uchar4[ size ];
	cl_uchar4* outputBuffer = new cl_uchar4[ size ];
	memset( imageBuffer, 0, size * sizeof( cl_uchar4 ) );
	memset( outputBuffer, 0, size * sizeof( cl_uchar4 ) );

	for( unsigned int y = 0; y < rawImage.getSize().y; y++ )
	{
		for( unsigned int x = 0; x < rawImage.getSize().x; x++ )
		{
			sf::Color color = rawImage.getPixel( x, y );
			cl_uchar4 clColor;

			clColor.s[ 0 ] = color.r;
			clColor.s[ 1 ] = color.g;
			clColor.s[ 2 ] = color.b; 
			clColor.s[ 3 ] = color.a;
			imageBuffer[ y * rawImage.getSize().x + x ] = clColor;
		}
	}

	bool showRawTexture = true;

	rawSprite.setTexture( gravel );
	gravelTiled.loadFromImage( tiledImage );
	rawSprite.setScale( scale, scale );

	size_t selectedPlatformIdx;
	size_t selectedDeviceIdx;
	OpenCLProgram program;
	program.InitializeCL();
	int i = 0;
	for( auto platform : program.Platforms() )
	{
		std::cout << "[ " << i++ << " ] " << platform->PlatformName() << std::endl;
	}
	cin >> selectedPlatformIdx;
	i = 0;
	for( auto device : ( program.Platforms()[ selectedPlatformIdx ]->Devices() ) )
	{
		std::cout << "[ " << i++ << " ] " << device->cl_device_name << " (" << ( device->CL_device_type == CL_DEVICE_TYPE_GPU ? "GPU" : "CPU" ) << ")" << std::endl;
	}
	cin >> selectedDeviceIdx;

	program.SelectPlatformAndDevice( selectedPlatformIdx, selectedDeviceIdx );
	program.LoadKernel( "CL/TextureTiler.cl", "MakeTile" );

	program.AddKernelArgGlobal( CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, size * sizeof( cl_uchar4 ), imageBuffer );
	program.AddKernelArgGlobal( CL_MEM_WRITE_ONLY , size * sizeof( cl_uchar4 ) );
	program.AddKernelArgInt( rawImage.getSize().x );
	program.AddKernelArgInt( rawImage.getSize().y );

	program.SetWorkSize<0>( 256 );
	int groupCount = size / 256;
	groupCount++;
	program.SetGroupCount<0>( groupCount );

	program.SetArgs();
	program.Run();
	//program.WaitForKernel();

	program.ReadOutput( 1, outputBuffer );

	for( unsigned int y = 0; y < tiledImage.getSize().y; y++ )
	{
		for( unsigned int x = 0; x < tiledImage.getSize().x; x++ )
		{
			cl_uchar4 clColor = outputBuffer[ y * tiledImage.getSize().x + x ];
			sf::Color color;
			color.r = clColor.s[ 0 ];
			color.g = clColor.s[ 1 ];
			color.b = clColor.s[ 2 ];
			color.a = clColor.s[ 3 ];
			tiledImage.setPixel( x, y, color );
		}
	}

	gravelTiled.loadFromImage( tiledImage );

	program.Release();

	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
				window.close();
			switch( event.type )
			{
			case sf::Event::KeyReleased:
				switch( event.key.code )
				{
				case sf::Keyboard::Subtract:
					scale /= 1.25f;
					break;

				case sf::Keyboard::Add:
					scale *= 1.25f;
					break;

				case sf::Keyboard::Numpad0:
					scale = 1.0f;
					break;
				case sf::Keyboard::Space:
					showRawTexture = !showRawTexture;
					rawSprite.setTexture( showRawTexture ? gravel : gravelTiled );
				default:
					break;
				}
			default:
				break;
			}
		}


		window.clear();

		rawSprite.setScale( scale, scale );

		sf::Vector2u size = gravel.getSize();
		size = sf::Vector2u( ( unsigned int ) ( size.x * scale ), ( unsigned int ) ( size.y * scale ));

		unsigned int xSize = 2;
		unsigned int ySize = 2;

		for( size_t x = 0; x < xSize; x++ )
		{
			for( size_t y = 0; y < ySize; y++ )
			{
				rawSprite.setPosition( ( float ) size.x * x, ( float ) size.y * y );
				window.draw( rawSprite );
			}
		}


		window.display();
	}

	return 0;
}

