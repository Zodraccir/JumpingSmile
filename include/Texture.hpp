#ifndef TEXTURE
#define TEXTURE

#include "common.hpp"

// wrapper class
class Texture
{
	public:
		//Initializes variables    
		Texture();

		//Deallocates memory
		~Texture();

		//Loads image at specified path
        bool loadFromFile( std::string path , SDL_Renderer* gRenderer);
		
        //Deallocates texture
		void free();

		//Renders texture at given point
		void render(SDL_Renderer* gRenderer);

		//Gets image dimensions
		int getWidth();
		int getHeight();

		
	private:
		//The actual hardware texture
		SDL_Texture* gTexture;
		SDL_RendererFlip flipType;

		//Image dimensions
		int mWidth;
		int mHeight;
		
		
};

#endif