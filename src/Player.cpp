#include "../include/Player.hpp"

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

Player::Player(int startPos)
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	frame=0;
	flipType = SDL_FLIP_NONE;
	mPosY=WALKING_Y_BASE;
	jumps=0;
	mPosX=startPos;

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

void Player::render(SDL_Renderer* gRenderer)
{
	//Set rendering space and render to screen
	
	renderQuad.x=mPosX;
	renderQuad.y=mPosY;
	renderQuad.w=mWidth;
	renderQuad.h=mHeight;

	
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

SDL_Rect* Player::getRenderGuad()
{
	return &renderQuad;
}
