#include <game/Player.h>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace game
{
	map<string, Player *> Player::players;
	
	Player::Player( const string& name ) : name(name), controller(NULL), eventHandler(NULL)
	{
		map<string, Player *>::iterator it = Player::players.find( name );
		
		if( it != Player::players.end() && it->second != NULL )
			delete it->second;
			
		Player::players[name] = this;
	
		#ifdef DEBUG1
		Logger::get() << "[Player#" << this->name << "] Initialized." << Logger::endl;
		#endif
	}
	
	Player::~Player()
	{
		if( this->eventHandler != NULL )
			delete this->eventHandler;
			
		if( this->controller != NULL )
			this->controller->setPlayer( NULL );
			
		#ifdef DEBUG1
		Logger::get() << "[Player#" << this->name << "] Destroyed." << Logger::endl;
		#endif
	}
	
	const string& Player::getName() const
	{
		return this->name;
	}
	
	void Player::handleEvent( Mapping::Button button, short int value, unsigned int timestamp )
	{
		if( this->eventHandler != NULL )
			this->eventHandler->handleEvent( this->controller, button, value, timestamp );
	}
	
	void Player::setEventHandler( EventHandler * eventHandler )
	{
		if( this->eventHandler != NULL )
			delete this->eventHandler;
		
		this->eventHandler = eventHandler;
	}
	
	void Player::setController( Controller * controller )
	{
		this->controller = controller;
		
		if( this->controller != NULL )
		{
			this->controller->setPlayer( this );

			#ifdef DEBUG1
			Logger::get() << "[Player#" << this->name << "] Linked to Controller#" << this->controller->getId() << "." << Logger::endl;
			#endif
		}
		#ifdef DEBUG1
		else
		{
			Logger::get() << "[Player#" << this->name << "] Detached from controller." << Logger::endl;
		}
		#endif
	}
	
	Player * Player::get( const string& name )
	{
		Player * player = NULL;
		
		map<string, Player *>::iterator it = Player::players.find( name );
		
		if( it != Player::players.end() )
			player = it->second;
		
		return player;
	}
	
	unsigned int Player::getCount()
	{
		return Player::players.size();
	}
	
	Player * Player::get( unsigned int index )
	{
		Player * player = NULL;
		
		for( map<string, Player *>::iterator it = Player::players.begin() ; it != Player::players.end() ; it++ )
		{
			if( index == 0 )
			{
				player = it->second;
				break;
			}
			
			index--;
		}
		
		return player;
	}
	
	void Player::destroy()
	{
		for( map<string, Player *>::iterator it = Player::players.begin() ; it != Player::players.end() ; it++ )
		{
			if( it->second != NULL )
				delete it->second;
		}
		
		Player::players.clear();
	}
}

