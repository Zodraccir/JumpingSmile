#include "../include/Bullet.hpp"


double distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

bool checkCollision( Circle& a, SDL_Rect* b )
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( a.x < b->x )
    {
        cX = b->x;
    }
    else if( a.x > b->x + b->w )
    {
        cX = b->x + b->w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest y offset
    if( a.y < b->y )
    {
        cY = b->y;
    }
    else if( a.y > b->y + b->h )
    {
        cY = b->y + b->h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if( distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

bool checkCollision( Circle& a, Circle& b )
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

// Bullet::Bullet( int x, int y )
// {
//     //Initialize the offsets
//     mPosX = x;
//     mPosY = y;

// 	//Set collision circle size
// 	mCollider.r = DOT_WIDTH / 2;

//     //Initialize the velocity
//     mVelX = 0;
//     mVelY = 0;

// 	//Move collider relative to the circle
// 	shiftColliders();
//     crashSound=NULL;
// }

Bullet::Bullet()
{
    

	//Set collision circle size
	mCollider.r = DOT_WIDTH / 2;

    reset();
    crashSound=NULL;

}

Bullet::~Bullet()
{
    free();
}

bool Bullet::reset()
{
    //Initialize the offsets
    //mPosX = mCollider.r;
    mPosY = mCollider.r + rand()%(SCREEN_HEIGHT-(SCREEN_HEIGHT * (2/3))  -   + 1) + (SCREEN_HEIGHT * (2/3));

    if( 0 + (rand() % (1 - 0 + 1)) == 1)
    {
        mPosX=mCollider.r;
        mVelX=DOT_VEL;
    }
    else
    {
        mPosX=SCREEN_WIDTH-mCollider.r;
        mVelX=-DOT_VEL;
    }

    //Initialize the velocity
    //mVelX = 0;
    mVelY = 0;

	//Move collider relative to the circle
	shiftColliders();
    return true;
}

void Bullet::free()
{   

    //Free the music
	if(crashSound!=NULL)
		Mix_FreeChunk( crashSound );
	crashSound=NULL;
    if(bTexture!=NULL)
        SDL_DestroyTexture(bTexture);
    bTexture=NULL;
}


int Bullet::move( SDL_Rect* square)
{
    //Move the dot left or right
    mPosX += mVelX;
	shiftColliders();

    //If the dot went too far to the left or right
	if( ( mPosX - mCollider.r < 0 ) || ( mPosX + mCollider.r > SCREEN_WIDTH ))
    {
        //Move back
        mPosX -= mVelX;
		shiftColliders();
        return 1;
    }
    
    //If the dot collided with the square
    if( checkCollision( mCollider, square ) )
    {
        Mix_PlayChannel( -1, crashSound, 0 );
        return -1;
    }

    return 0;

    //Move the dot up or down
    mPosY += mVelY;
	shiftColliders();

    //If the dot collided or went too far up or down
    if( ( mPosY - mCollider.r < 0 ) || ( mPosY + mCollider.r > SCREEN_HEIGHT ) || checkCollision( mCollider, square )  )
    {
        //Move back
        mPosY -= mVelY;
		shiftColliders();
    }
}


bool Bullet::loadFromFile( std::string path , SDL_Renderer* gRenderer)
{

    SDL_Texture* newTexture=NULL;
    loadedSurface = IMG_Load( path.c_str() );
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
	bTexture = newTexture;
	return bTexture != NULL;    
}

void Bullet::render(SDL_Renderer* gRenderer)
{
    SDL_Rect renderQuad = { mPosX - mCollider.r, mPosY - mCollider.r, DOT_WIDTH, DOT_HEIGHT };

	//Render to screen
	SDL_RenderCopyEx( gRenderer, bTexture, NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE );

}

Circle& Bullet::getCollider()
{
	return mCollider;
}

void Bullet::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}


void Bullet::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: mVelY -= DOT_VEL; break;
            case SDLK_s: mVelY += DOT_VEL; break;
            case SDLK_a: mVelX -= DOT_VEL; break;
            case SDLK_d: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_w: mVelY += DOT_VEL; break;
            case SDLK_s: mVelY -= DOT_VEL; break;
            case SDLK_a: mVelX += DOT_VEL; break;
            case SDLK_d: mVelX -= DOT_VEL; break;
        }
    }
}

Mix_Chunk* Bullet::getCrashSound()
{
	return crashSound;
}

bool Bullet::setCrashSound(Mix_Chunk* sound)
{	
	crashSound = sound;
	if(crashSound == NULL)
	{
		return false;
	}
	return true;
}