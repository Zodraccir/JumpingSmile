#include "../include/GameEngine.hpp"
#include <iostream>

GameEngine::GameEngine()
{
	gWindow=NULL;
	gRenderer=NULL;
	// for(int i=0;i<5;i++)
	// {
	// 	bullet[i]=new Bullet();
	// }
	
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
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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

					//Initialize SDL_mixer
					if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
					{
						printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
	//bullet->free();
	for(int i=0;i<5;i++)
	{
		bullet[i].free();
	}
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
	if( !player->loadFromFile( "assets/player.png" , "assets/black.png" , gRenderer ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	if( player->setJumpMusic(Mix_LoadWAV( "assets/jump.wav" )) == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	if( !texture.loadFromFile( "assets/background.png" , gRenderer ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	if( texture.setMusic(Mix_LoadMUS( "assets/musicLoop.wav" )) == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	

	if( !startTexture.loadFromFile( "assets/prompt.png" , gRenderer ) )
	{
		printf( "Failed to load starting texture!\n" );
		success = false;
	}

	if( startTexture.setMusic(Mix_LoadMUS( "assets/landingMusic.wav" )) == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	for(int i=0;i<5;i++)
	{
		//Load dot texture
		if( !bullet[i].loadFromFile( "assets/dot.bmp" , gRenderer ) )
		{
			printf( "Failed to load dot texture!\n" );
			success = false;
		}

		if( bullet[i].setCrashSound(Mix_LoadWAV( "assets/crash.wav" )) == NULL )
		{
			printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
			success = false;
		}

		if( bullet[i].setLoseSound(Mix_LoadWAV( "assets/lose.wav" )) == NULL )
		{
			printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
			success = false;
		}
	}

	// //Load dot texture
	// if( !bullet->loadFromFile( "assets/dot.bmp" , gRenderer ) )
	// {
	// 	printf( "Failed to load dot texture!\n" );
	// 	success = false;
	// }

	// if( bullet->setCrashSound(Mix_LoadWAV( "assets/crash.wav" )) == NULL )
	// {
	// 	printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	// 	success = false;
	// }

	//Open the font
	gFont = TTF_OpenFont( "assets/lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	
	
	return success;
}

bool GameEngine::landingPage()
{
	bool quit=false;

	//If there is no music playing
	if( Mix_PlayingMusic() == 0 )
	{
		//Play the music
		Mix_PlayMusic( startTexture.getMusic(), -1 );
	}


	while( !quit )
	{
		while(SDL_PollEvent ( &e) !=0 )
		{
			if(e.type==SDL_QUIT)
			{
				return false;
			}
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
			{
				//If there is no music playing
				if( Mix_PlayingMusic() != 0 )
				{
					//Play the music
					Mix_HaltMusic();
				}
				return true;
			}
		}
		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		startTexture.render(gRenderer);
		

		//Update screen
		SDL_RenderPresent( gRenderer );

	}
	startTexture.free();
}

bool GameEngine::loop()
{
    //Main loop flag
	bool quit = false;
	int score=0;
	//If there is no music playing
	if( Mix_PlayingMusic() == 0 )
	{
		//Play the music
		Mix_PlayMusic( texture.getMusic(), -1 );
	}


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
			//bullet->handleEvent( e );
		}
		
		//Move the player
		player->move();

		//std::cout<<player->toString()<<std::endl;
		
		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		
		texture.render(gRenderer);
		player->render(gRenderer);
		
		
		for(int i=0;i<5;i++)
		{	
			int bulletMove=bullet[i].move(player->getRenderGuad() ,player->getRenderGuadFist() );
			if(bulletMove == 0){
				bullet[i].render(gRenderer);
				continue;
			}

			else if(bulletMove  <0 ){
				player->rockHitted(-bulletMove);
				score+=6+bulletMove;
			}
			else if (bulletMove == 10){
				quit=true;
			}
			bullet[i].reset();
		}
		
	
		
		// switch (bullet->move( player->getRenderGuad() ))
		// {
		// case 0:
		// 	bullet->render(gRenderer);
		// 	break;
		// case -1:
		// 	score++;
		// case 1:
		// default:
		// 	bullet->reset();
		// }
		

		counter.render(gRenderer,gFont, std::to_string(score));

		
		//Update screen
		SDL_RenderPresent( gRenderer );

				
	}
}