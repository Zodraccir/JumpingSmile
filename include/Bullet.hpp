#ifndef BULLET
#define BULLET

#include "common.hpp"

//A circle stucture
struct Circle
{
    int x, y;
    int r;
};

// wrapper class
class Bullet
{
    public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 1;

        Bullet();

        //Initializes the variables
        Bullet( int x, int y );

        ~Bullet();

        //Deallocates texture
		void free();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot and checks collision
        void move( SDL_Rect* square);

        //Loads image at specified path
        bool loadFromFile( std::string path , SDL_Renderer* gRenderer);

        //Shows the dot on the screen
        void render(SDL_Renderer* gRenderer);

        //Gets collision circle
        Circle& getCollider();

    private:
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
        
        //Dot's collision circle
        Circle mCollider;

        //Image dimensions
		int mWidth;
		int mHeight;

        //Moves the collision circle relative to the dot's offset
        void shiftColliders();
		
        SDL_Texture* bTexture;
        
        SDL_Surface* loadedSurface;
};


#endif
