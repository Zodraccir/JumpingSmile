#ifndef COUNTER
#define COUNTER

#include "common.hpp"

// wrapper class
class Counter
{
	public:
		//Initializes variables    
		Counter();

		//Deallocates memory
		~Counter();
		
        //Deallocates texture
		void free();

		//Renders texture at given point
        void render(SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textToRender);

		
	private:
		

        //Set rendering space and render to screen
        SDL_Rect renderQuad;
        SDL_Color textColor;

        //Texture for text
        SDL_Texture* textTexture;

        SDL_Surface * textSurface;
		
};
#endif
