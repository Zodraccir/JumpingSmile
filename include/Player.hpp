#ifndef PLAYER
#define PLAYER

#include "common.hpp"
#include "Fist.hpp"

// wrapper class
class Player
{
	public:
		
		static const int PLAYER_VEL = 10;
		//The dimensions of the player
		static const int PLAYER_WIDTH = 64;
		static const int PLAYER_HEIGHT = 205;
		
		//Initializes variables    
		Player();

		Player(int startPos);
		//Deallocates memory
		~Player();

		//Loads image at specified path
        bool loadFromFile( std::string path , std::string pathFist , SDL_Renderer* gRenderer);
		
        //Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render(SDL_Renderer* gRenderer);

		//Gets image dimensions
		int getWidth();
		int getHeight();
		

		void move();
		void handleEvent( SDL_Event& e );

		//GetNumberOfJumps
		int getJumps();

        int getmPosX();
        int getmPosY();

		SDL_Rect* getRenderGuad();
		SDL_Rect* getRenderGuadFist();

		bool setJumpMusic(Mix_Chunk* sound);
		Mix_Chunk* getJumpMusic();

		bool isNotJumping();

		std::string toString();

		
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		SDL_RendererFlip flipType;

		SDL_Rect renderQuad;

		//frame
		SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

		//frame
		SDL_Rect gSpriteClipsWhileJump[ WALKING_ANIMATION_FRAMES ];

		//Image dimensions
		int mWidth;
		int mHeight;
		int frame;

		//base y walking
		int WALKING_Y_BASE;
		
		//The position of the player
		int mPosX,mPosY;

		//The velocity of the player
		int mVelX,mVelY;
		
		//Number of Jumps made
		int jumps;

		//If the player is lowere to the ground
		int lowered;

		int maxJump;

		Mix_Chunk *jumpSound;

		Fist* fist;
};

#endif