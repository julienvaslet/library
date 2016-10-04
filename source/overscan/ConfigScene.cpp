#include <overscan/ConfigScene.h>
#include <overscan/ConfigSceneEventHandler.h>
#include <controller/Controller.h>
#include <opengl/ui/Font.h>
#include <opengl/Screen.h>

#include <tools/pi/Overscan.h>
#include <sstream>

using namespace controller;
using namespace opengl;
using namespace opengl::ui;
using namespace tools::pi;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace overscan
{
	ConfigScene::ConfigScene() : Scene(), lastDrawTicks(0), top(0), left(0), right(0), bottom(0), step(0)
	{
		Overscan::get( &(this->left), &(this->right), &(this->top), &(this->bottom) );
		
		this->updateStrings();
		this->initializeStep();
		
		// Initialize controllers event handler
		for( unsigned int i = 0 ; i < Player::getCount() ; i++ )
			Player::get( i )->setEventHandler( new ConfigSceneEventHandler( this ) );
	}
	
	ConfigScene::~ConfigScene()
	{
		for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
			delete *it;
			
		this->rectangles.clear();
	}
	
	void ConfigScene::updateStrings()
	{
		stringstream ss;
		
		ss.str( "" );
		ss << this->top;
		this->sTop = ss.str();
		
		ss.str( "" );
		ss << this->left;
		this->sLeft = ss.str();
		
		ss.str( "" );
		ss << this->right;
		this->sRight = ss.str();
		
		ss.str( "" );
		ss << this->bottom;
		this->sBottom = ss.str();
	}
	
	void ConfigScene::updateOverscan()
	{
		Overscan::set( &(this->left), &(this->right), &(this->top), &(this->bottom) );
		this->updateStrings();
		this->initializeStep( this->step );
	}
	
	void ConfigScene::initializeRectangles()
	{
		// Create a rectangle to keep shaders loaded
		Color white( "ffffff" );
		ColoredRectangle * rectangle = new ColoredRectangle( 0, 0, white );
		
		// Clear old rectangles
		for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
			delete (*it);
			
		this->rectangles.clear();
		
		unsigned int screenWidth = Screen::get()->getWidth() - this->left - this->right;
		unsigned int screenHeight = Screen::get()->getHeight() - this->top - this->bottom;
		
		// Top
		rectangle->resize( screenWidth, 10 );
		rectangle->getOrigin().moveTo( this->left, this->top, 0.0f );
		this->rectangles.push_back( rectangle );
		
		// Left
		rectangle = new ColoredRectangle( 10, screenHeight, white );
		rectangle->getOrigin().moveTo( this->left, this->top, 0.0f );
		this->rectangles.push_back( rectangle );
		
		// Bottom
		rectangle = new ColoredRectangle( screenWidth, 10, white );
		rectangle->getOrigin().moveTo( this->left, this->top + screenHeight - 10, 0.0f );
		this->rectangles.push_back( rectangle );
		
		// Right
		rectangle = new ColoredRectangle( 10, screenHeight, white );
		rectangle->getOrigin().moveTo( this->left + screenWidth - 10, this->top, 0.0f );
		this->rectangles.push_back( rectangle );
	}
	
	void ConfigScene::initializeStep( int step )
	{
		// Top-left corner
		switch( step )
		{
			case 0:
			{
				this->instructions = "Make the top-left\ncorner visible";
				
				this->initializeRectangles();
		
				// Left-top corner
				ColoredRectangle * rectangle = new ColoredRectangle( 10, 100 );
				rectangle->getOrigin().moveTo( this->left, this->top, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
		
				rectangle = new ColoredRectangle( 100, 10 );
				rectangle->getOrigin().moveTo( this->left, this->top, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
				
				this->step = 0;
				break;
			}
			
			case 1:
			{
				this->instructions = "Make the bottom-right\ncorner visible";
				
				this->initializeRectangles();
		
				// Right-bottom corner
				ColoredRectangle * rectangle = new ColoredRectangle( 10, 100 );
				rectangle->getOrigin().moveTo( Screen::get()->getWidth() - this->right - 10, Screen::get()->getHeight() - this->bottom - 100, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
		
				rectangle = new ColoredRectangle( 100, 10 );
				rectangle->getOrigin().moveTo( Screen::get()->getWidth() - this->right - 100, Screen::get()->getHeight() - this->bottom - 10, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
				
				this->step = 1;
				break;
			}
			
			case 2:
			{
				this->running = false;
			}
		}
	}
	
	void ConfigScene::handleEvent( SDL_Event * event )
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

	void ConfigScene::moveDown( unsigned int times )
	{
		switch( this->step )
		{
			case 0:
			{
				this->top += times;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->bottom -= times;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::moveUp( unsigned int times )
	{
		switch( this->step )
		{
			case 0:
			{
				this->top -= times;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->bottom += times;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::moveLeft( unsigned int times )
	{
		switch( this->step )
		{
			case 0:
			{
				this->left -= times;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->right += times;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::moveRight( unsigned int times )
	{
		switch( this->step )
		{
			case 0:
			{
				this->left += times;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->right -= times;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::nextStep()
	{
		this->step++;
		this->initializeStep( this->step );
	}
	
	void ConfigScene::previousStep()
	{
		if( this->step > 0 )
			this->step--;
			
		this->initializeStep( this->step );
	}

	void ConfigScene::live( unsigned int ticks )
	{
		Controller::tickEvent( ticks );
	}
	
	void ConfigScene::render( unsigned int ticks )
	{
		Point2D textSize;
		vector<Point3D> vertices;
		vector<Color> colors;
		vector<unsigned int short> indices;
		
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
				(*it)->prepareRendering( vertices, colors, indices );
				
			ColoredRectangle::render( vertices, colors, indices );
			
			unsigned int screenWidth = Screen::get()->getWidth() - this->left - this->right;
			unsigned int screenHeight = Screen::get()->getHeight() - this->top - this->bottom;
			
			// Top
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sTop );
			Font::get("bitmap")->write( Point2D( this->left + (screenWidth - textSize.getX()) / 2.0f, this->top + 20 ), this->sTop );
			
			// Left
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sLeft );
			Font::get("bitmap")->write( Point2D( this->left + 20, this->top + (screenHeight - textSize.getY()) / 2.0f ), this->sLeft );
			
			// Right
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sRight );
			Font::get("bitmap")->write( Point2D( this->left + screenWidth - textSize.getX() - 20, this->top + (screenHeight - textSize.getY()) / 2.0f ), this->sRight );
			
			// Bottom
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sBottom );
			Font::get("bitmap")->write( Point2D( this->left + (screenWidth - textSize.getX()) / 2.0f, this->top + screenHeight - textSize.getY() - 20 ), this->sBottom );
			
			// Instructions
			textSize.moveTo( 0, this->top + (screenHeight - Font::get("bitmap")->getTextHeight( this->instructions ) ) / 2.0f );
			
			string line;
			unsigned int index = 0;
			size_t newLineIndex = this->instructions.find( '\n', index );
			
			while( newLineIndex != string::npos )
			{
				line = this->instructions.substr( index, newLineIndex - index );
				textSize.setX( this->left + (screenWidth - Font::get("bitmap")->getTextWidth( line )) / 2.0f );
				
				Font::get("bitmap")->write( textSize, line );
				textSize.moveBy( 0, Font::get("bitmap")->getTextHeight( line ) );
				
				index = newLineIndex + 1;
				newLineIndex = this->instructions.find( '\n', index );
			}
			
			if( index < this->instructions.length() )
			{
				line = this->instructions.substr( index );
				textSize.setX( this->left + (screenWidth - Font::get("bitmap")->getTextWidth( line )) / 2.0f );
				Font::get("bitmap")->write( textSize, line );
			}
			
			Font::get("bitmap")->render();
			
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
}

