#include <blockgame/IntroScene.h>
#include <blockgame/PlayScene.h>
#include <blockgame/IntroSceneEventHandler.h>

#include <opengl/OpenGL.h>
#include <opengl/ui/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>

#include <controller/Controller.h>
#include <vector>

using namespace opengl;
using namespace opengl::ui;
using namespace controller;
using namespace std;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600	

namespace blockgame
{
	IntroScene::IntroScene() : Scene(), lastDrawTicks(0), lastAlphaChangeTicks(0), increaseAlpha(false), textColor(1.0f, 1.0f, 1.0f)
	{
		this->textColor.setAlpha( 1.0f );
		
		Player * player = Player::get( "Player" );
		
		if( player != NULL )
			player->setEventHandler( new IntroSceneEventHandler( this ) );

		else
		{
			#ifdef DEBUG1
			Logger::get() << "[IntroScene] No player found. Exiting." << Logger::endl;
			#endif
			
			this->running = false;
		}
	}
	
	IntroScene::~IntroScene()
	{
	}
	
	void IntroScene::handleEvent( SDL_Event * event )
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
	
	void IntroScene::live( unsigned int ticks )
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
	
	void IntroScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			Point2D origin;
			
			string title = "Block Game !";
			vector<Color> titleColors;
			titleColors.push_back( Color( "ff0000" ) ); // B
			titleColors.push_back( Color( "00ff00" ) ); // l
			titleColors.push_back( Color( "0000ff" ) ); // o
			titleColors.push_back( Color( "ff00ff" ) ); // c
			titleColors.push_back( Color( "00ffff" ) ); // k
			titleColors.push_back( Color( "000000" ) ); // _
			titleColors.push_back( Color( "ffffff" ) ); // G
			titleColors.push_back( Color( "ff0000" ) ); // a
			titleColors.push_back( Color( "00ff00" ) ); // m
			titleColors.push_back( Color( "0000ff" ) ); // e
			titleColors.push_back( Color( "000000" ) ); // _
			titleColors.push_back( Color( "ff00ff" ) ); // !
			
			Font::get("bitmap")->getTextSize( origin, title, 2.0f );
			origin.moveTo( (SCREEN_WIDTH - origin.getX()) / 2.0f, 150 );
			
			for( unsigned int i = 0 ; i < title.length() ; i++ )
			{
				Font::get("bitmap")->write( origin, title.substr( i, 1 ), titleColors[i], 2.0f );
				origin.moveBy( Font::get("bitmap")->getTextWidth( title.substr( i, 1 ), 2.0f ), 0.0f );
			}
			
			origin.moveTo( 0.0f, 0.0f );
			Font::get("bitmap")->getTextSize( origin, "Press any button to start", 1.0f );
			Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - origin.getX()) / 2.0f, SCREEN_HEIGHT - origin.getY() - 200 ), "Press any button to start", this->textColor, 1.0f );
			
			Font::get("bitmap")->render();
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
	
	void IntroScene::endScene()
	{
		this->running = false;
		this->nextScene = new PlayScene();
	}
}

