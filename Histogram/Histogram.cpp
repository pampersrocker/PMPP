// TextureTiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "OpenCL.h"
#include "CalcStatisticKernelWrapper.h"

typedef ReferenceCounted< OpenCLKernel_tpl<1> > OpenCLKernelPtr;


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window( sf::VideoMode( 1280, 800), "Histogram" );

	sf::Texture inputTexture;
	sf::Texture gravelTiled;
	sf::Sprite rawSprite;

	sf::Vector2i imageOffset;
	sf::Vector2i initialOffset;
	bool offsetActive = false;

	sf::Image tiledImage;
	if( !inputTexture.loadFromFile( "Gravel.jpg" ) )
	{
		std::cout << "Failed to load Gravel.jpg!" << std::endl;
	}
	sf::Image rawImage = inputTexture.copyToImage();

	tiledImage.create( rawImage.getSize().x, rawImage.getSize().y );

	float scale = 1.0f;

	

	rawSprite.setTexture( inputTexture );
	gravelTiled.loadFromImage( tiledImage );
	rawSprite.setScale( scale, scale );

	OpenCLManager clManager;
	clManager.Initialize();
	auto* device = clManager.ConsoleSelectPlatformAndDevice();

	auto clContext = device->CreateContext();

	OpenCLKernelPtr kernel = clContext->CreateKernel<1>( "CL/Histogram.cl", "calcStatistic" );
	OpenCLKernelPtr reduceStatistic = clContext->CreateKernel<1>( "CL/ReduceStatistic.cl", "reduceStatistic" );



	CalcStatisticKernelWrapper wrapper( kernel, reduceStatistic );

	wrapper.SetImage( rawImage );

	wrapper.Run();

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

		for( size_t x = 0; x < xSize; x++ )
		{
			for( size_t y = 0; y < ySize; y++ )
			{
				rawSprite.setPosition( ( float ) size.x * x + imageOffset.x, ( float ) size.y * y + imageOffset.y );
				window.draw( rawSprite );
			}
		}


		window.display();
	}

	return 0;
}

