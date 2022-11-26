#include "../include/Counter.hpp"


Counter::Counter()
{
  
    renderQuad = { 550, 10, 60, 60 };
    textColor = { 0, 0, 0, 0xFF };
    textTexture = NULL;
    textSurface = NULL;
}

Counter::~Counter()
{
	//Deallocate
	free();
}

//Deallocates texture
void Counter::free()
{

}

//Renders texture at given point
void Counter::render(SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textToRender)
{

	textSurface = TTF_RenderText_Solid( gFont, textToRender.c_str(), textColor );
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
}

