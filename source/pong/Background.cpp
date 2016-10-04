#include <pong/Background.h>
#include <pong/Constants.h>

namespace pong
{
	Background::Background()
	{
		// Walls color
		Color white( "ffffff" );
		ColoredRectangle * rect = NULL;
		
		// Left wall
		rect = new ColoredRectangle( WALL_SIZE, SCREEN_HEIGHT, white );
		rect->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
		this->walls.push_back( rect );
		
		// Top wall
		rect = new ColoredRectangle( SCREEN_WIDTH, WALL_SIZE, white );
		rect->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
		this->walls.push_back( rect );
		
		// Right wall
		rect = new ColoredRectangle( WALL_SIZE, SCREEN_HEIGHT, white );
		rect->getOrigin().moveTo( SCREEN_WIDTH - WALL_SIZE, 0.0f, 0.0f );
		this->walls.push_back( rect );
		
		// Bottom wall
		rect = new ColoredRectangle( SCREEN_WIDTH, WALL_SIZE, white );
		rect->getOrigin().moveTo( 0.0f, SCREEN_HEIGHT - WALL_SIZE, 0.0f );
		this->walls.push_back( rect );
		
		// Center lines
		for( unsigned int y = WALL_SIZE ; y < SCREEN_HEIGHT ; y += WALL_SIZE * 4.0f )
		{
			rect = new ColoredRectangle( WALL_SIZE, WALL_SIZE * 2.0f, white );
			rect->getOrigin().moveTo( (SCREEN_WIDTH - WALL_SIZE) / 2.0f, y, 0.0f );
			this->walls.push_back( rect );
		}
	}
	
	Background::~Background()
	{
		for( vector<ColoredRectangle *>::iterator it = this->walls.begin() ; it != this->walls.end() ; it++ )
			delete *it;
			
		this->walls.clear();
	}
	
	void Background::prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices )
	{
		for( vector<ColoredRectangle *>::iterator it = this->walls.begin() ; it != this->walls.end() ; it++ )
			(*it)->prepareRendering( vertices, colors, indices );
	}
}

