#include "../include/Fist.hpp"


Fist::Fist(int x,int y)
{   
   
	
    //Initialize
	fTexture = NULL;
	mWidth = 30;
	mHeight = 30;
	flipType = SDL_FLIP_NONE;
	fistSound=NULL;

	mPosY=x;
	mPosX=y;
	mVelX=0;
	mVelY=0;


}

Fist::~Fist()
{
	//Deallocate
	free();
}


void Fist::free()
{
	//Free texture if it exists
	if( fTexture != NULL )
	{
		SDL_DestroyTexture( fTexture );
		fTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
	//Free the music
	if(fistSound!=NULL)
		Mix_FreeChunk( fistSound );
	fistSound=NULL;
}

void Fist::render(SDL_Renderer* gRenderer)
{
	//Set rendering space and render to screen
	
	renderQuad.x=mPosX;
	renderQuad.y=mPosY;
	renderQuad.w=mWidth;
	renderQuad.h=mHeight;
	//Render to screen


	SDL_RenderCopyEx( gRenderer, fTexture, NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE );

}

//Loads image at specified path
bool Fist::loadFromFile( std::string path , SDL_Renderer* gRenderer)
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
	fTexture = newTexture;
	return fTexture != NULL;
}


void Fist::move(int x,int y, SDL_RendererFlip fliptype)
{
    //Move the dot left or right mPosX+mWidth,mPosY-(mHeight/2)
    mPosX = x;
    mPosY = y;
    //std::cout<<std::to_string(mPosX)+"/"+std::to_string(mPosY)+"/"+std::to_string(mVelX)+"/"+std::to_string(mVelY)+"/"+std::to_string(mHeight)+" :Fist"<<std::endl;
	flipType=flipType;
}

std::string Fist::toString()
{

	return std::to_string(mPosX)+"/"+std::to_string(mPosY)+"/"+std::to_string(mVelX)+"/"+std::to_string(mVelY)+"/"+std::to_string(mWidth)+" :Fist1";
}

SDL_Rect* Fist::getRenderGuad()
{
	return &renderQuad;
}
