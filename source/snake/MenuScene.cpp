#include <snake/MenuScene.h>
#include <snake/PlayScene.h>
#include <snake/MenuSceneEventHandler.h>
#include <snake/Constants.h>
#include <snake/Map.h>

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

namespace snake
{
	MenuScene::MenuScene() : Scene(), lastDrawTicks(0), selectIcon(NULL), selectedAction(0), selectingMap(false)
	{	
		Player * player = Player::get( "Player" );
		
		if( player != NULL )
			player->setEventHandler( new MenuSceneEventHandler( this ) );

		else
		{
			#ifdef DEBUG1
			Logger::get() << "[MenuScene] No player found. Exiting." << Logger::endl;
			#endif
			
			this->running = false;
		}
		
		Color white("ffffff");
		this->selectIcon = new ColoredRectangle( SELECT_ICON_SIZE, SELECT_ICON_SIZE, white );
		this->selectIcon->getAnchor().moveTo( SELECT_ICON_SIZE / 2.0f, SELECT_ICON_SIZE / 2.0f );
		
		// Actions
		this->actions.push_back( "New game" );
		this->actions.push_back( "Quit" );
		
		// Maps
		this->maps = Map::getAvailableMaps();
		
		#ifdef DEBUG1
		Logger::get() << "[MenuScene] Found " << this->maps.size() << " map(s)." << Logger::endl;
		#endif
	}
	
	MenuScene::~MenuScene()
	{
	}
	
	void MenuScene::handleEvent( SDL_Event * event )
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
	
	void MenuScene::live( unsigned int ticks )
	{
	}
	
	void MenuScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			string titleStr = "Snake";
			Point2D size( 0.0f, 0.0f );
			Font::get("bitmap")->getTextSize( size, titleStr, 2.0f );
			Font::get("bitmap")->write( Point2D( (SCREEN_WIDTH - size.getX()) / 2.0f, 150.0f ), titleStr, 2.0f );
			
			// Actions
			Point2D origin( 0.0f, MENU_ORIGIN_Y );
			
			if( !this->selectingMap )
			{
				// Determine longest actions string
				for( vector<string>::const_iterator it = this->actions.begin() ; it != this->actions.end() ; it++ )
				{
					size.moveTo( 0.0f, 0.0f );
					Font::get("bitmap")->getTextSize( size, *it );
				
					if( origin.getX() < size.getX() )
						origin.setX( size.getX() );
				}
			
				origin.setX( (SCREEN_WIDTH - origin.getX()) / 2.0f );
			
				// Writing actions
				unsigned int i = 0;
				for( vector<string>::const_iterator it = this->actions.begin() ; it != this->actions.end() ; it++, i++ )
				{
					size.moveTo( 0.0f, 0.0f );
					Font::get("bitmap")->getTextSize( size, *it );
					Font::get("bitmap")->write( origin, *it );
				
					if( i == this->selectedAction )
					{
						vector<Point3D> vertices;
						vector<Color> colors;
						vector<unsigned short int> indices;
					
						this->selectIcon->getOrigin().moveTo( origin.getX() - 2.0f * SELECT_ICON_SIZE, origin.getY() + ( size.getY() / 2.0f ), 0.0f );
						this->selectIcon->prepareRendering( vertices, colors, indices );
						ColoredRectangle::render( vertices, colors, indices );
					}
				
					origin.moveBy( 0.0f, size.getY() );
				}
			}
			else
			{
				// Determine longest map name
				for( vector<string>::const_iterator it = this->maps.begin() ; it != this->maps.end() ; it++ )
				{
					size.moveTo( 0.0f, 0.0f );
					Font::get("bitmap")->getTextSize( size, *it );
				
					if( origin.getX() < size.getX() )
						origin.setX( size.getX() );
				}
			
				origin.setX( (SCREEN_WIDTH - origin.getX()) / 2.0f );
				
				// Writing maps' name
				unsigned int i = 0;
				for( vector<string>::const_iterator it = this->maps.begin() ; it != this->maps.end() ; it++, i++ )
				{
					size.moveTo( 0.0f, 0.0f );
					Font::get("bitmap")->getTextSize( size, *it );
					Font::get("bitmap")->write( origin, *it );
				
					if( i == this->selectedAction )
					{
						vector<Point3D> vertices;
						vector<Color> colors;
						vector<unsigned short int> indices;
					
						this->selectIcon->getOrigin().moveTo( origin.getX() - 2.0f * SELECT_ICON_SIZE, origin.getY() + ( size.getY() / 2.0f ), 0.0f );
						this->selectIcon->prepareRendering( vertices, colors, indices );
						ColoredRectangle::render( vertices, colors, indices );
					}
				
					origin.moveBy( 0.0f, size.getY() );
				}
			}
			
			Font::get("bitmap")->render();
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
	
	void MenuScene::selectLowerAction( int times )
	{
		this->selectedAction += times;
		
		if( !this->selectingMap )
		{
			if( this->selectedAction >= this->actions.size() )
				this->selectedAction = this->actions.size() - 1;
		}
		else
		{
			if( this->selectedAction >= this->maps.size() )
				this->selectedAction = this->maps.size() - 1;
		}
	}
	
	void MenuScene::selectUpperAction( int times )
	{
		if( this->selectedAction >= static_cast<unsigned int>( times ) )
			this->selectedAction -= times;
		else
			this->selectedAction = 0;
	}
	
	void MenuScene::selectCurrentAction()
	{
		if( !this->selectingMap )
		{
			switch( this->selectedAction )
			{
				// Switch to map-selection
				case 0:
				{
					this->selectedAction = 0;
					this->selectingMap = true;
					break;
				}
			
				// Quit
				case 1:
				{
					this->running = false;
					this->nextScene = NULL;
					break;
				}
			
				default:
					break;
			}
		}
		else
		{
			// Start game
			this->running = false;
			this->nextScene = new PlayScene( this->maps.at( this->selectedAction ) );
		}
	}
}

