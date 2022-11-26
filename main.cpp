#include "include/common.hpp"
#include "include/GameEngine.hpp"

int main( int argc, char* args[] )
{
	//Start up SDL and create window

	GameEngine gameEngine;

	if( !gameEngine.init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !gameEngine.loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			gameEngine.loop();
		}
	}

	//Free resources and close SDL
	gameEngine.close();

	return 0;
}
