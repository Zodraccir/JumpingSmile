#include "../include/GameEngine.hpp"

GameEngine::GameEngine()
{

}

GameEngine::~GameEngine()
{

}

bool GameEngine::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				else
				{
					//Initialize SDL_ttf
					if( TTF_Init() == -1 )
					{
						printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
						success = false;
					}

				}
				
			}
		}
	}

	return success;
}

void GameEngine::close()
{
	//Free loaded images
	player.free();
	texture.free();
	//Free Font
	TTF_CloseFont( gFont );
	gFont = NULL;
	
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool GameEngine::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !player.loadFromFile( "assets/foo.png" , gRenderer ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}

	if( !texture.loadFromFile( "assets/background.png" , gRenderer ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}


	//Open the font
	gFont = TTF_OpenFont( "assets/lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	
	
	return success;
}


bool GameEngine::loop()
{
    //Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;
			
	//Angle of rotation
	double degrees = 0;

	//Flip type
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			
			//Handle input for the player
			player.handleEvent( e );
		}

				//Move the dot
		player.move();


				//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );
				
				//Render texture to screen
		texture.render(gRenderer);

			
		counter.render(gRenderer,gFont, std::to_string(player.getmPosY()));

				//Render current frame
		player.render(gRenderer);

			

				//Update screen
		SDL_RenderPresent( gRenderer );

				
	}
}