#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int WALKING_ANIMATION_FRAMES = 4;

const int WALKING_Y_BASE = ( SCREEN_HEIGHT ) / 2;

// wrapper class
class Player
{
	public:
		
		static const int PLAYER_VEL = 10;
		//The dimensions of the player
		static const int PLAYER_WIDTH = 64;
		static const int PLAYER_HEIGHT = 205;
		
		//Initializes variables    
		Player();

		//Deallocates memory
		~Player();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render();

		//Gets image dimensions
		int getWidth();
		int getHeight();
		
		SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
		
		void move();
		void handleEvent( SDL_Event& e );

		//GetNumberOfJumps
		int getJumps();
		
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		SDL_RendererFlip flipType;

		//Image dimensions
		int mWidth;
		int mHeight;
		int frame;
		
		//The position of the player
		int mPosX,mPosY;

		//The velocity of the player
		int mVelX,mVelY;
		
		//Salti fatti
		int jumps;

		
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//Text Surface
SDL_Surface* textSurface = NULL;

//Set rendering space and render to screen
SDL_Rect renderQuad = { 550, 10, 60, 60 };

SDL_Color textColor = { 0, 0, 0, 0xFF };

//Texture for text
SDL_Texture* textTexture;

//Texture
SDL_Texture* gTexture = NULL;

//Font
TTF_Font *gFont = NULL;


//Walking animation

SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
Player player;


Player::Player()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	frame=0;
	flipType = SDL_FLIP_NONE;
	mPosY=WALKING_Y_BASE;
	jumps=0;
}

Player::~Player()
{
	//Deallocate
	free();
}

bool Player::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void Player::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		frame= 0;
	}
}

void Player::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void Player::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void Player::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void Player::render()
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { mPosX, mPosY , mWidth, mHeight };
	
	SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
	
	if(mVelX<0)
		flipType = SDL_FLIP_NONE;
	else if(mVelX>0)
		flipType = SDL_FLIP_HORIZONTAL;
	
	//Set clip rendering dimensions
	if( currentClip != NULL )
	{
		renderQuad.w = currentClip->w;
		renderQuad.h = currentClip->h;
	}

	//Render to screen
	//SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
	SDL_RenderCopyEx( gRenderer, mTexture, currentClip, &renderQuad, 0, NULL, flipType );
	
	//Go to next frame
	++frame;

	//Cycle animation
	if( frame / 4 >= WALKING_ANIMATION_FRAMES )
	{
		frame = 0;
	}
}

int Player::getWidth()
{
	return mWidth;
}

int Player::getHeight()
{
	return mHeight;
}

int Player::getJumps()
{
	return jumps;
}



void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
            case SDLK_SPACE:
            	if(mPosY>=WALKING_Y_BASE)
            	{
            		mVelY=PLAYER_VEL;
					jumps++;
            	}
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelX += PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
        }
    }
  
}

void Player::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + PLAYER_VEL > SCREEN_WIDTH - PLAYER_WIDTH) )
    {
        //Move back
        mPosX -= mVelX;
    }
  
  
    if( mPosY > WALKING_Y_BASE-150  || mPosY < WALKING_Y_BASE)
        mPosY-=mVelY;
    //If the dot went too far up or down
    if( ( mPosY < WALKING_Y_BASE-150 ) )
    	mVelY=-PLAYER_VEL*1.7;
    if( mPosY >= WALKING_Y_BASE){
    	mVelY=0;
		mPosY=WALKING_Y_BASE;
	}
}





bool init()
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


SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}


bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !player.loadFromFile( "assets/foo.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else
	{
		//Set sprite clips
		player.gSpriteClips[ 0 ].x =   0;
		player.gSpriteClips[ 0 ].y =   0;
		player.gSpriteClips[ 0 ].w = Player::PLAYER_WIDTH;
		player.gSpriteClips[ 0 ].h = Player::PLAYER_HEIGHT;

		player.gSpriteClips[ 1 ].x = Player::PLAYER_WIDTH;
		player.gSpriteClips[ 1 ].y =   0;
		player.gSpriteClips[ 1 ].w = Player::PLAYER_WIDTH;
		player.gSpriteClips[ 1 ].h = Player::PLAYER_HEIGHT;
		
		player.gSpriteClips[ 2 ].x = Player::PLAYER_WIDTH*2;
		player.gSpriteClips[ 2 ].y =   0;
		player.gSpriteClips[ 2 ].w = Player::PLAYER_WIDTH;
		player.gSpriteClips[ 2 ].h = Player::PLAYER_HEIGHT;

		player.gSpriteClips[ 3 ].x = Player::PLAYER_WIDTH*3;
		player.gSpriteClips[ 3 ].y =   0;
		player.gSpriteClips[ 3 ].w = Player::PLAYER_WIDTH;
		player.gSpriteClips[ 3 ].h = Player::PLAYER_HEIGHT;
	}
	
	gTexture = loadTexture( "assets/background.png" );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
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

void close()
{
	//Free loaded images
	player.free();
	SDL_DestroyTexture(gTexture);

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

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
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
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
				

				std::string stringa =  std::to_string(player.getJumps());
				
				textSurface = TTF_RenderText_Solid( gFont, stringa.c_str(), textColor );
				if( textSurface != NULL )
				{
						//Create texture from surface pixels
					textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
					if( textTexture == NULL )
						{
							printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
					}
					//Get rid of old surface
					SDL_FreeSurface( textSurface );
					}
				else
				{
					printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
				}

				//Render to screen
				SDL_RenderCopyEx( gRenderer, textTexture, NULL , &renderQuad, 0.0, NULL, SDL_FLIP_NONE );

				
				//Render current frame
				player.render();
				

				//Update screen
				SDL_RenderPresent( gRenderer );

				
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
