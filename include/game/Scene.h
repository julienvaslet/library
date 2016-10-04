#ifndef __GAME_SCENE_H
#define __GAME_SCENE_H	1

#include <SDL2/SDL.h>

namespace game
{
	class Scene
	{	
		protected:
			bool running;
			Scene * nextScene;
			
		public:
			Scene();
			virtual ~Scene();
			
			virtual void handleEvent( SDL_Event * event ) = 0;
			virtual void live( unsigned int ticks ) = 0;
			virtual void render( unsigned int ticks ) = 0;
			
			bool isRunning() const;
			Scene * getNextScene();
	};
}

#endif
