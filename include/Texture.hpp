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

		bool setMusic(Mix_Music* music);
		Mix_Music* getMusic();

	private:
		//The actual hardware texture
		SDL_Texture* gTexture;
		SDL_RendererFlip flipType;

		//Music associate with texturebackground
		Mix_Music *gMusic;

		//Image dimensions
		int mWidth;
		int mHeight;
		
		
};

#endif