// TextureTiler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window( sf::VideoMode( 1280, 900), "Texture Tiler" );

	sf::Texture gravel;
	sf::Sprite rawSprite;

	if( !gravel.loadFromFile( "Gravel.jpg" ) )
	{
		std::cout << "Failed to load Gravel.png!" << std::endl;
	}

	float scale = 1.0f;

	rawSprite.setTexture( gravel );


	rawSprite.setScale( scale, scale );

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

