// TextureTiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "OpenCL.h"
#include "CalcStatisticKernelWrapper.h"
#include "Logging/BPPDefaultConsoleLogger.hpp"


typedef ReferenceCounted< OpenCLKernel_tpl<1> > OpenCLKernelPtr;


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window( sf::VideoMode( 1280, 800), "Histogram" );

	sf::Texture inputTexture;
	sf::Texture histogramTexture;
	sf::Sprite rawSprite;
	sf::Sprite histogramSprite;

	sf::Vector2i imageOffset;
	sf::Vector2i initialOffset;
	bool offsetActive = false;

	sf::Image histogramImage;
	if( !inputTexture.loadFromFile( "Gravel.jpg" ) )
	{
		std::cout << "Failed to load Gravel.jpg!" << std::endl;
	}
	sf::Image rawImage = inputTexture.copyToImage();

	histogramImage.create( 258,258, sf::Color::Transparent );

	float scale = 1.0f;

	

	rawSprite.setTexture( inputTexture );
	rawSprite.setScale( scale, scale );

	OpenCLManager clManager;
	clManager.Initialize();
	auto* device = clManager.ConsoleSelectPlatformAndDevice();

	auto clContext = device->CreateContext();

	OpenCLKernelPtr kernel = clContext->CreateKernel<1>( "CL/Histogram.cl", "calcStatistic" );
	OpenCLKernelPtr reduceStatistic = clContext->CreateKernel<1>( "CL/ReduceStatistic.cl", "reduceStatistic" );


	{
		HistogramGPUScenario scenario( kernel, reduceStatistic, rawImage );
		auto& benchmarker = bpp::Benchmarker::Instance();

		bpp::DefaultConsoleLogger logger;

		benchmarker.AddLogger( &logger );
		benchmarker.Iterations( 5 );

		benchmarker.AddScenario( &scenario );

		benchmarker.Run();
		benchmarker.Log();

		benchmarker.Release();
	}

	CalcStatisticKernelWrapper wrapper( kernel, reduceStatistic );

	wrapper.SetImage( rawImage );

	wrapper.Run();

	for( size_t i = 0; i < 258; i++ )
	{
		histogramImage.setPixel( 0, i, sf::Color(255, 255, 255 ) );
		histogramImage.setPixel( 257, i, sf::Color(255, 255, 255 ) );
		histogramImage.setPixel( i, 0, sf::Color(255, 255, 255 ) );
		histogramImage.setPixel( i, 257, sf::Color(255, 255, 255 ) );
	}
	
	std::array<int, 256> result;

	wrapper.ReadOutput( result );

	int max = result[0];
	for( size_t i = 1; i < 256; i++ )
	{
		if( result[i] > max )
		{
			max = result[ i ];
		}
	}

	for( size_t x = 0; x < 256; x++ )
	{
		for( size_t y = 0; y < 256; y++ )
		{
			if( ((float)result[ x ] / (float)max) * 255 >= y )
			{
				histogramImage.setPixel( x + 1, 256 - y , sf::Color::White );
			}
		}
	}

	histogramTexture.loadFromImage( histogramImage );
	histogramSprite.setTexture( histogramTexture );
	histogramSprite.setPosition( 1280.0f - 258.0f, 200 );
	clManager.Release();

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
				default:
					break;
				}
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Button::Right )
				{
					offsetActive = true;
					initialOffset = sf::Vector2i( event.mouseButton.x, event.mouseButton.y ) - imageOffset;
				}
				break;

			case sf::Event::MouseButtonReleased:
				if( event.mouseButton.button == sf::Mouse::Button::Right )
				{
					offsetActive = false;
				}
				break;

			case sf::Event::MouseMoved:
				if (offsetActive)
				{
					imageOffset = sf::Vector2i( event.mouseMove.x , event.mouseMove.y ) - initialOffset;
				}
				break;
			case sf::Event::MouseWheelMoved:
				scale *= 1.0f + (0.25f * event.mouseWheel.delta);
				break;
			default:
				break;
			}
		}


		window.clear();

		rawSprite.setScale( scale, scale );

		sf::Vector2u size = inputTexture.getSize();
		size = sf::Vector2u( ( unsigned int ) ( size.x * scale ), ( unsigned int ) ( size.y * scale ));

		unsigned int xSize = 5;
		unsigned int ySize = 5;

		
		rawSprite.setPosition( imageOffset.x, imageOffset.y );
		window.draw( rawSprite );
		window.draw( histogramSprite );


		window.display();
	}

	return 0;
}

