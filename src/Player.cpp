#include "../include/Player.hpp"


Player::Player()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	frame=0;
	flipType = SDL_FLIP_NONE;
	
	jumps=0;
	jumpSound=NULL;

	WALKING_Y_BASE = ( SCREEN_HEIGHT ) / 3 + PLAYER_HEIGHT;

	mPosY=WALKING_Y_BASE;
	mPosX=10;
	mVelX=0;
	mVelY=0;

	maxJump=WALKING_Y_BASE- (( SCREEN_HEIGHT ) / 3);

	
	lowered=false;
}

Player::Player(int startPos)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	frame=0;
	flipType = SDL_FLIP_NONE;
	
	jumps=0;
	jumpSound=NULL;

	WALKING_Y_BASE = ( SCREEN_HEIGHT ) / 3 + PLAYER_HEIGHT;

	mPosY=WALKING_Y_BASE;
	mPosX=startPos;
	mVelX=0;
	mVelY=0;
	
	maxJump=WALKING_Y_BASE- (( SCREEN_HEIGHT ) / 3);
	
	lowered=false;
}

Player::~Player()
{
	//Deallocate
	free();
}

bool Player::loadFromFile( std::string path , SDL_Renderer* gRenderer)
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

			//Set sprite clips to move in player class
	gSpriteClips[ 0 ].x =   0;
	gSpriteClips[ 0 ].y =   0;
	gSpriteClips[ 0 ].w = PLAYER_WIDTH;
	gSpriteClips[ 0 ].h = PLAYER_HEIGHT;

	gSpriteClips[ 1 ].x = PLAYER_WIDTH;
	gSpriteClips[ 1 ].y =   0;
	gSpriteClips[ 1 ].w = PLAYER_WIDTH;
	gSpriteClips[ 1 ].h = PLAYER_HEIGHT;
		
	gSpriteClips[ 2 ].x = PLAYER_WIDTH*2;
	gSpriteClips[ 2 ].y =   0;
	gSpriteClips[ 2 ].w = PLAYER_WIDTH;
	gSpriteClips[ 2 ].h = PLAYER_HEIGHT;

	gSpriteClips[ 3 ].x = PLAYER_WIDTH*3;
	gSpriteClips[ 3 ].y =   0;
	gSpriteClips[ 3 ].w = PLAYER_WIDTH;
	gSpriteClips[ 3 ].h = PLAYER_HEIGHT;


	//Set sprite clips to move in player class
	gSpriteClipsWhileJump[ 0 ].x = PLAYER_WIDTH*4;
	gSpriteClipsWhileJump[ 0 ].y =   0;
	gSpriteClipsWhileJump[ 0 ].w = PLAYER_WIDTH;
	gSpriteClipsWhileJump[ 0 ].h = PLAYER_HEIGHT/2;

	gSpriteClipsWhileJump[ 1 ].x = PLAYER_WIDTH*5;
	gSpriteClipsWhileJump[ 1 ].y =   0;
	gSpriteClipsWhileJump[ 1 ].w = PLAYER_WIDTH;
	gSpriteClipsWhileJump[ 1 ].h = PLAYER_HEIGHT/2;
		
	gSpriteClipsWhileJump[ 2 ].x = PLAYER_WIDTH*6;
	gSpriteClipsWhileJump[ 2 ].y =   0;
	gSpriteClipsWhileJump[ 2 ].w = PLAYER_WIDTH;
	gSpriteClipsWhileJump[ 2 ].h = PLAYER_HEIGHT/2;

	gSpriteClipsWhileJump[ 3 ].x = PLAYER_WIDTH*7;
	gSpriteClipsWhileJump[ 3 ].y =   0;
	gSpriteClipsWhileJump[ 3 ].w = PLAYER_WIDTH;
	gSpriteClipsWhileJump[ 3 ].h = PLAYER_HEIGHT/2;


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
	//Free the music
	if(jumpSound!=NULL)
		Mix_FreeChunk( jumpSound );
	jumpSound=NULL;
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

void Player::render(SDL_Renderer* gRenderer)
{
	//Set rendering space and render to screen
	
	renderQuad.x=mPosX;
	renderQuad.y=mPosY;
	renderQuad.w=mWidth;
	renderQuad.h=mHeight;

	
	SDL_Rect* currentClip = NULL;

	if(abs(mVelY)>0 || lowered==true)
	{
		currentClip = &gSpriteClipsWhileJump[ frame / 4 ];
	}
	else
	{
		currentClip = &gSpriteClips[ frame / 4 ];
	}
	
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
	if(abs(mVelX)>0)
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

int Player::getmPosX()
{
	return mPosX;
}

int Player::getmPosY()
{
	return mPosY;
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
				if(isNotJumping())
				{
            		mVelY=PLAYER_VEL;
					jumps++;
					Mix_PlayChannel( -1, jumpSound, 0 );
				}
				break;
			case SDLK_LCTRL:
			
            	if(isNotJumping())
            	{
            		lowered=true;
					mPosY=mPosY+Player::PLAYER_HEIGHT/2;
            	}
				else if (lowered==true)
				{
					lowered=false;
					mPosY=mPosY-Player::PLAYER_HEIGHT/2;
				}
				break;
				
				
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
  
	if(abs(mVelY)>0)
	{	
		
			if(mPosY>WALKING_Y_BASE)
			{
				mPosY=WALKING_Y_BASE;
				mVelY=0;
			}
			else if(mPosY<=maxJump)
			{
				mVelY=-mVelY;
			}
			mPosY-=mVelY;
	}


}

SDL_Rect* Player::getRenderGuad()
{
	return &renderQuad;
}

Mix_Chunk* Player::getJumpMusic()
{
	return jumpSound;
}

bool Player::setJumpMusic(Mix_Chunk* sound)
{	
	jumpSound = sound;
	if(jumpSound == NULL)
	{
		return false;
	}
	return true;
}

bool Player::isNotJumping()
{
	if(mPosY==WALKING_Y_BASE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string Player::toString()
{
	return std::to_string(mPosX)+"/"+std::to_string(mPosY)+"/"+std::to_string(mVelX)+"/"+std::to_string(mVelY)+"/"+std::to_string(lowered);
}