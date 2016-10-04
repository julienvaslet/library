#include <blockgame/ScoreScene.h>
#include <blockgame/IntroScene.h>
#include <blockgame/ScoreSceneEventHandler.h>

#include <opengl/OpenGL.h>
#include <opengl/ui/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>

#include <controller/Controller.h>

#include <sstream>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace opengl;
using namespace opengl::ui;
using namespace controller;

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600	

namespace blockgame
{
	ScoreScene::ScoreScene( unsigned int score ) : Scene(), lastDrawTicks(0), lastAlphaChangeTicks(0), increaseAlpha(false), textColor(1.0f, 1.0f, 1.0f)
	{
		Player * player = Player::get( "Player" );
		
		if( player != NULL )
			player->setEventHandler( new ScoreSceneEventHandler( this ) );
			
		else
		{
			#ifdef DEBUG1
			Logger::get() << "[ScoreScene] No player found. Exiting." << Logger::endl;
			#endif
			
			this->running = false;
		}
		
		this->textColor.setAlpha( 1.0f );
		stringstream ss;
		ss << "Your score is : " << score;
		this->score = ss.str();
	}
	
	ScoreScene::~ScoreScene()
	{
	}
	
	void ScoreScene::handleEvent( SDL_Event * event )
	{
		switch( event->type )
		{
			case SDL_QUIT:
			{
				this->running = false;
				break;
			}
			
			#ifndef __PI__
			case SDL_KEYDOWN:
			{
	            if( event->key.keysym.sym == SDLK_ESCAPE )
					this->running = false;
				
				break;
			}
			#endif
			
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			{
				Controller::handleEvent( event );
				break;
			}
		}
	}
	
	void ScoreScene::live( unsigned int ticks )
	{
		if( ticks - this->lastAlphaChangeTicks > 30 )
		{
			float alpha = this->textColor.getAlpha();
			
			if( increaseAlpha )
			{
				alpha += 0.1f * (static_cast<float>( this->lastAlphaChangeTicks ) / static_cast<float>( ticks ));
				
				if( alpha > 1.0f )
				{
					alpha = 1.0f;
					increaseAlpha = false;
				}
			}
			else
			{
				alpha -= 0.1f * (static_cast<float>( this->lastAlphaChangeTicks ) / static_cast<float>( ticks ));
				
				if( alpha < 0.0f )
				{
					alpha = 0.0f;
					increaseAlpha = true;
				}
			}
			
			this->textColor.setAlpha( alpha );
			this->lastAlphaChangeTicks = ticks;
		}
	}
	
	void ScoreScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			Point2D origin;
			
			Font::get("bitmap")->getTextSize( origin, this->score, 1.5f );
			Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - origin.getX()) / 2.0f, 150 ), this->score, 1.5f );
			
			origin.moveTo( 0.0f, 0.0f );
			Font::get("bitmap")->getTextSize( origin, "Press any button to quit", 1.0f );
			Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - origin.getX()) / 2.0f, SCREEN_HEIGHT - origin.getY() - 200 ), "Press any button to quit", this->textColor, 1.0f );
			
			Font::get("bitmap")->render();
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
	
	void ScoreScene::endScene()
	{
		this->running = false;
		this->nextScene = new IntroScene();
	}
}

