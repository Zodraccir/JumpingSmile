#include "../include/GameEngine.hpp"
#include <iostream>

GameEngine::GameEngine()
{
	gWindow=NULL;
	gRenderer=NULL;
	bullet = new Bullet(20,30);
	
}

GameEngine::~GameEngine()
{
}


bool GameEngine::init()
{
	//Initialization flag
	bool success = true;

	player= new Player(240);

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
	player->free();
	texture.free();
	bullet->free();
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
	if( !player->loadFromFile( "assets/foo.png" , gRenderer ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}

	if( !texture.loadFromFile( "assets/background.png" , gRenderer ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}

	//Load dot texture
	if( !bullet->loadFromFile( "assets/dot.bmp" , gRenderer ) )
	{
		printf( "Failed to load dot texture!\n" );
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
	
	//The dot that will be moving around on the screen
	Bullet bullet1( 300, 300);

			//Set the wall
			SDL_Rect wall;
			wall.x = 300;
			wall.y = 200;
			wall.w = 1;
			wall.h = 1;

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
			player->handleEvent( e );
			bullet->handleEvent( e );
		}
		
		//Move the player
		player->move();

		
		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		
		texture.render(gRenderer);
		player->render(gRenderer);


		bullet->move( player->getRenderGuad() );
		//Render texture to screen
		
		bullet->render(gRenderer);

		counter.render(gRenderer,gFont, std::to_string(player->getmPosY()));


		//Render current frame
		

			

		//Update screen
		SDL_RenderPresent( gRenderer );

				
	}
}