#ifndef GAME_ENGINE
#define GAME_ENGINE

#include "common.hpp"
#include "Player.hpp"
#include "Texture.hpp"
#include "Counter.hpp"
#include "Bullet.hpp"

// wrapper class
class GameEngine
{
	public:
                //Initializes variables    
                GameEngine();

                //Deallocates memory
                ~GameEngine();
                        
                //Starts up SDL and creates window
                bool init();

                //Loads media
                bool loadMedia();

                bool loop();

                //Frees media and shuts down SDL
                void close();

        
	private:
                //The window we'll be rendering to
                SDL_Window* gWindow = NULL;

                //The window renderer
                SDL_Renderer* gRenderer = NULL;

                //Font
                TTF_Font *gFont = NULL;

                Player* player;
                Texture texture;
                Counter counter;

                Bullet* bullet;

		
};
#endif
