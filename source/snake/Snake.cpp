#include <snake/Snake.h>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace snake
{
	Snake::Snake() : origin(0.0f, 0.0f), direction(1.0f, 0.0f, 0.0f), directionHasChanged(false), color("ffffff")
	{
		ColoredRectangle * head = new ColoredRectangle( SNAKE_WIDTH, SNAKE_WIDTH, this->color );
		head->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
		this->rectangles.push_back( head );
	}
	
	Snake::~Snake()
	{
		for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
			delete (*it);
			
		this->rectangles.clear();
	}
			
	void Snake::moveTo( int x, int y )
	{
		vector<ColoredRectangle *>::iterator it = this->rectangles.begin();
		
		if( it != this->rectangles.end() )
		{
			int newX = GAME_AREA_X + x * SNAKE_WIDTH;
			int newY = GAME_AREA_Y + y * SNAKE_WIDTH;
			int dx = newX - (*it)->getOrigin().getX();
			int dy = newY - (*it)->getOrigin().getY();
		
			for( ; it != this->rectangles.end() ; it++ )
				(*it)->getOrigin().moveBy( dx, dy, 0.0f );
		
			this->keepTheTailInArea();
		}
	}
	
	void Snake::move( unsigned int times )
	{
		for( unsigned int i = 0 ; i < times ; i++ )
		{
			for( vector<ColoredRectangle *>::reverse_iterator it = this->rectangles.rbegin() ; it != this->rectangles.rend() ; )
			{
				ColoredRectangle * current = (*it);
				it++;
				
				if( it != this->rectangles.rend() )
					current->getOrigin().moveTo( (*it)->getOrigin().getX(), (*it)->getOrigin().getY(), (*it)->getOrigin().getZ() );
				else
				{
					// Head case
					current->getOrigin().moveBy( this->direction.getX() * SNAKE_WIDTH, this->direction.getY() * SNAKE_WIDTH, 0.0f );
				}
			}
			
			this->directionHasChanged = false;
		}
		
		this->keepTheTailInArea();
	}
	
	void Snake::keepTheTailInArea()
	{
		for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
		{
			// Horizontal replacement
			if( (*it)->getOrigin().getX() < GAME_AREA_X )
				(*it)->getOrigin().setX( GAME_AREA_X + GAME_AREA_WIDTH - SNAKE_WIDTH );
			else if( (*it)->getOrigin().getX() >= GAME_AREA_X + GAME_AREA_WIDTH )
				(*it)->getOrigin().setX( GAME_AREA_X );
			
			// Vertical replacement
			if( (*it)->getOrigin().getY() < GAME_AREA_Y )
				(*it)->getOrigin().setY( GAME_AREA_Y + GAME_AREA_HEIGHT - SNAKE_WIDTH );
			else if( (*it)->getOrigin().getY() >= GAME_AREA_Y + GAME_AREA_HEIGHT )
				(*it)->getOrigin().setY( GAME_AREA_Y );
		}
	}
	
	void Snake::setDirection( int dx, int dy )
	{
		// Avoid more than one change by move
		if( !this->directionHasChanged )
		{
			// Pseudo normalization
			if( dx > 0 && dx > 1 ) dx = 1;
			if( dx < 0 && dx < -1 ) dx = -1;
			if( dy > 0 && dy > 1 ) dy = 1;
			if( dy < 0 && dy < -1 ) dy = -1;
		
			// Avoid inverse-direction
			if( ( this->direction.getX() == dx && this->direction.getY() == -dy ) || ( this->direction.getX() == -dx && this->direction.getY() == dy ) )
				return;
		
			this->direction.setX( dx );
			this->direction.setY( dy );
			this->directionHasChanged = true;
		}
	}
	
	void Snake::grow( unsigned int times )
	{
		int tailX = 0;
		int tailY = 0;
		
		vector<ColoredRectangle *>::reverse_iterator it = this->rectangles.rbegin();
		
		if( it != this->rectangles.rend() )
		{
			tailX = (*it)->getOrigin().getX();
			tailY = (*it)->getOrigin().getY();
		}
		
		for( unsigned int i = 0 ; i < times ; i++ )
		{
			this->move();
			
			ColoredRectangle * tail = new ColoredRectangle( SNAKE_WIDTH, SNAKE_WIDTH, this->color );
			tail->getOrigin().moveTo( tailX, tailY, 0.0f );
			this->rectangles.push_back( tail );
		}
	}
	
	bool Snake::isInCollision( const ColoredRectangle& rectangle ) const
	{
		bool collision = false;
		
		for( vector<ColoredRectangle *>::const_iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
		{
			if( rectangle.isInCollision( **it ) )
			{
				collision = true;
				break;
			}
		}
		
		return collision;
	}
	
	// This test should be encaspulated in a "vector" rectangle class..
	bool Snake::isInCollision( const Map& map ) const
	{
		bool collision = false;
		
		for( vector<ColoredRectangle *>::const_iterator itSnake = this->rectangles.begin() ; itSnake != this->rectangles.end() ; itSnake++ )
		{
			for( vector<ColoredRectangle *>::const_iterator itMap = map.getMap().begin() ; itMap != map.getMap().end() ; itMap++ )
			{
				if( (*itMap)->isInCollision( **itSnake ) )
				{
					collision = true;
					break;
				}
			}
			
			if( collision )
				break;
		}
		
		return collision;
	}
	
	bool Snake::isBitingItsTail() const
	{
		bool collision = false;
		
		vector<ColoredRectangle *>::const_iterator it = this->rectangles.begin();
		ColoredRectangle * head = (*it);
		it++;
		
		for( ; it != this->rectangles.end() ; it++ )
		{
			ColoredRectangle * tail = *it;
			if( head->isInCollision( *tail ) )
			{
				collision = true;
				break;
			}
		}
		
		return collision;
	}
	
	void Snake::prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices ) const
	{
		vector<ColoredRectangle *>::const_iterator it = this->rectangles.begin();
		(*it)->prepareRendering( vertices, colors, indices );
		
		Color green("00ff00");
		colors[ colors.size() - 1 ] = green;
		colors[ colors.size() - 2 ] = green;
		colors[ colors.size() - 3 ] = green;
		colors[ colors.size() - 4 ] = green;
		
		it++;
		
		for(  ; it != this->rectangles.end() ; it++ )
			(*it)->prepareRendering( vertices, colors, indices );
	}
}
