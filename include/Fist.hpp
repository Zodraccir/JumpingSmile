#ifndef FIST
#define FIST

#include "common.hpp"

// wrapper class
class Fist
{
	public:
		
		static const int FIST_VEL = 10;
		//The dimensions of the player

		
		//Initializes variables    
		Fist(int x,int y);

		//Deallocates memory
		~Fist();
		
        //Deallocates texture
		void free();

		//Renders texture at given point
		void render(SDL_Renderer* gRenderer);

		//Gets image dimensions
		int getWidth();
		int getHeight();
		
		void move();

        int getmPosX();
        int getmPosY();

		SDL_Rect* getRenderGuad();

		bool setFistChuck(Mix_Chunk* sound);
		Mix_Chunk* getFistChuck();

		std::string toString();

		
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		SDL_RendererFlip flipType;

		SDL_Rect renderQuad;

        //Image dimensions
		int mWidth;
		int mHeight;

		
		//The position of the fist
		int mPosX,mPosY;

		//The velocity of the fist
		int mVelX,mVelY;
		
		Mix_Chunk *jumpSound;
        
        static const int PLAYER_WIDTH = 64;
		static const int PLAYER_HEIGHT = 205;
};

#endif