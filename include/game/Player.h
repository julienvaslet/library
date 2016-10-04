#ifndef __GAME_PLAYER_H
#define __GAME_PLAYER_H	1

namespace game
{
	class Player;
}

#include <controller/Controller.h>
#include <controller/EventHandler.h>

#include <map>
#include <string>

using namespace controller;
using namespace std;

namespace game
{
	class Player
	{
		protected:
			static map<string, Player *> players;
			
			string name;
			Controller * controller;
			EventHandler * eventHandler;
			
		public:
			Player( const string& name );
			virtual ~Player();
			
			const string& getName() const;
			void handleEvent( Mapping::Button button, short int value, unsigned int timestamp );
			void setEventHandler( EventHandler * eventHandler );
			void setController( Controller * controller );
			
			static Player * get( const string& name );
			static unsigned int getCount();
			static Player * get( unsigned int index );
			static void destroy();
	};
}

#endif

