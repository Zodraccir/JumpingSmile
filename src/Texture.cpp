#include "../include/Texture.hpp"

Texture::Texture()
{
	//Initialize
	gTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	flipType = SDL_FLIP_NONE;
	gMusic=NULL;

}

Texture::~Texture()
{
	//Deallocate
	free();
}
//Loads image at specified path
bool Texture::loadFromFile( std::string path , SDL_Renderer* gRenderer)
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
	gTexture = newTexture;
	return gTexture != NULL;
}
		
//Deallocates texture
void Texture::free()
{

	//Free the music
	if(gMusic!=NULL)
		Mix_FreeMusic( gMusic );
	gMusic = NULL;
	if(gTexture!=NULL)
		SDL_DestroyTexture(gTexture);
	gTexture=NULL;
}

//Renders texture at given point
void Texture::render(SDL_Renderer* gRenderer)
{

    //If music is being played
	SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
}

//Gets image dimensions
int Texture::getWidth()
{
    return mWidth;
}

int Texture::getHeight()
{
    return mHeight;
}

Mix_Music* Texture::getMusic()
{
	return gMusic;
}

bool Texture::setMusic(Mix_Music* music)
{	
	gMusic= music;
	if(gMusic == NULL)
	{
		return false;
	}
	return true;
}