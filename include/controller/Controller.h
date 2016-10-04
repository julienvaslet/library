#ifndef __CONTROLLER_CONTROLLER_H
#define __CONTROLLER_CONTROLLER_H	1

#include <SDL2/SDL.h>
#include <map>
#include <string>

#include <controller/Mapping.h>

namespace controller
{
	class Controller;
}

#include <game/Player.h>

using namespace game;
using namespace std;

namespace controller
{
	class Controller
	{
		protected:
			static map<SDL_JoystickID, Controller *> controllers;
			static map<string, Mapping *> mappings;
			
			SDL_Joystick * joystick;
			SDL_JoystickID id;
			Mapping * mapping;
			Player * player;
			
			map<Mapping::Button, pair<short int, unsigned int> > states;
		
			Controller( SDL_Joystick * joystick, const string& mapping );
			virtual ~Controller();
			
			bool loadMapping( const string& mapping );
			void updateState( Mapping::Button button, short int value, unsigned int timestamp );
			
		public:
			static void scan();
			static void destroy();
			static bool open( const string& name );
			static bool open( const string& name, const string& mapping );
			static void tickEvent( unsigned int timestamp );
			static void handleEvent( const SDL_Event * event );
			static unsigned int getControllersCount();
			static Controller * getFreeController();
			static Controller * get( SDL_JoystickID id );
			
			unsigned int getId() const;
			const char * getName() const;
			short int getState( Mapping::Button button );
			unsigned int getStateTimestamp( Mapping::Button button );
			
			void setPlayer( Player * player );
			Player * getPlayer();
	};
}

#endif

