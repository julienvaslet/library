#include <snake/PlayScene.h>
#include <snake/Constants.h>
#include <snake/PlaySceneEventHandler.h>
#include <snake/HighscoreScene.h>

#include <opengl/OpenGL.h>
#include <opengl/ui/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>
#include <opengl/Vector.h>

#include <game/Player.h>
#include <controller/Controller.h>
#include <audio/Mixer.h>
#include <audio/Song.h>
#include <audio/instrument/Sine.h>

#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace opengl;
using namespace opengl::ui;
using namespace controller;
using namespace std;
using namespace audio;
using namespace audio::instrument;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace snake
{
	PlayScene::PlayScene( const string& map ) : Scene(), lastDrawTicks(0), lastMoveTicks(0), score(0), speed(SNAKE_MIN_SPEED), paused(false), growUp(false), map(NULL), snake(NULL), apple(NULL)
	{
		Player * player = Player::get( "Player" );
		
		if( player != NULL )
			player->setEventHandler( new PlaySceneEventHandler( this ) );

		if( player == NULL )
		{
			#ifdef DEBUG1
			Logger::get() << "[PlayScene] No player found. Exiting." << Logger::endl;
			#endif
			
			this->running = false;
		}
		
		this->map = new Map( map );
		
		// Initializing random seed
		srand( time(NULL) );
		
		Logger::get() << "Origin: " << this->map->getSnakeOrigin().getX() << ", " << this->map->getSnakeOrigin().getY() << Logger::endl;
		
		this->snake = new Snake();
		this->snake->moveTo( this->map->getSnakeOrigin().getX(), this->map->getSnakeOrigin().getY() );
		this->snake->setDirection( 1, 0 );
		this->snake->grow( SNAKE_START_LENGTH - 1 );
		
		this->generateApple();
		
		this->updateStrings();
		this->lastMoveTicks = SDL_GetTicks();
	}
	
	PlayScene::~PlayScene()
	{
		if( this->map != NULL )
			delete this->map;
		
		if( this->snake != NULL )
			delete this->snake;
			
		if( this->apple != NULL )
			delete this->apple;
	}
	
	void PlayScene::updateStrings()
	{
		stringstream ss;
		
		ss.str( "" );
		ss << this->score;
		this->sScore = ss.str();
	}
	
	void PlayScene::handleEvent( SDL_Event * event )
	{
		switch( event->type )
		{
			case SDL_QUIT:
			{
				this->running = false;
				break;
			}
		
			case SDL_JOYAXISMOTION:
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			{
				Controller::handleEvent( event );
				break;
			}
		}
	}
	
	void PlayScene::generateApple()
	{
		if( this->apple == NULL )
		{
			Color red("ff0000");
			this->apple = new ColoredRectangle( SNAKE_WIDTH, SNAKE_WIDTH, red );
		}
		
		unsigned int tries = 10;
		
		do
		{
			int x = (rand() % (GAME_AREA_WIDTH / SNAKE_WIDTH)) * SNAKE_WIDTH + GAME_AREA_X;
			int y = (rand() % (GAME_AREA_HEIGHT / SNAKE_WIDTH)) * SNAKE_WIDTH + GAME_AREA_Y;
			this->apple->getOrigin().moveTo( x, y, 0.0f );
		}
		while( tries-- > 0 && ( this->snake->isInCollision( *(this->apple) ) || this->map->isInCollision( *(this->apple) ) ) );
		
		// No places found for the apple
		if( tries == 0 )
		{
			delete this->apple;
			this->apple = NULL;
		}
	}
	
	void PlayScene::live( unsigned int ticks )
	{
		Controller::tickEvent( ticks );
		
		if( ticks - this->lastMoveTicks > this->speed )
		{
			if( this->growUp )
			{
				this->growUp = false;
				this->snake->grow();
			}
			else
				this->snake->move();
			
			// Buggy
			if( this->snake->isBitingItsTail() || this->snake->isInCollision( *(this->map) ) )
			//if( this->snake->isBitingItsTail() )
				this->gameOver();
				
			if( this->apple != NULL )
			{
				if( this->snake->isInCollision( *this->apple ) )
				{
					this->score += SCORE_INCREMENT;
					this->generateApple();
					this->growUp = true;

					// speed up
					if( this->speed > SNAKE_MAX_SPEED )
						this->speed -= 1;
					
					this->updateStrings();
				}
			}
			else
				this->generateApple();
			
			this->lastMoveTicks = ticks;
		}
	}
	
	void PlayScene::render( unsigned int ticks )
	{
		vector<Point3D> vertices;
		vector<Color> colors;
		vector<unsigned int short> indices;
		
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
				
			this->map->prepareRendering( vertices, colors, indices );
			
			if( this->apple != NULL )
				this->apple->prepareRendering( vertices, colors, indices );
			
			this->snake->prepareRendering( vertices, colors, indices );
			
			ColoredRectangle::render( vertices, colors, indices );
			
			Font::get("bitmap")->write( Point2D( 10.0f, 10.0f ), "Score:" );
			Font::get("bitmap")->write( Point2D( 130.0f, 10.0f ), this->sScore );
			
			Font::get("bitmap")->render();
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}

	void PlayScene::moveUp()
	{
		this->snake->setDirection( 0, -1 );
	}
	
	void PlayScene::moveDown()
	{
		this->snake->setDirection( 0, 1 );
	}
	
	void PlayScene::moveLeft()
	{
		this->snake->setDirection( 1, 0 );
	}
	
	void PlayScene::moveRight()
	{
		this->snake->setDirection( -1, 0 );
	}
	
	void PlayScene::gameOver()
	{
		this->running = false;
		this->nextScene = new HighscoreScene( this->map->getTitle(), this->score );
	}
	
	void PlayScene::abortGame()
	{
		this->running = false;
		this->nextScene = NULL;
	}
}

