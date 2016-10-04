#include <snake/Map.h>
#include <snake/Constants.h>

#include <opengl/Color.h>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <dirent.h>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace opengl;

namespace snake
{
	Map::Map( const string& title ) : title(title), snakeOrigin(10, 10)
	{
		#ifdef DEBUG1
		Logger::get() << "[Map] Loading \"" << title << "\"..." << Logger::endl;
		#endif
		
		Color grey( "888888" );
		
		stringstream filename;
		filename << MAPS_DIRECTORY << "/" << title;
		
		ifstream file( filename.str() );
		
		if( file.is_open() )
		{
			unsigned int x = 0;
			unsigned int y = 0;
			string line;
			
			while( getline( file, line ) )
			{
				for( x = 0 ; x < line.length() ; x++ )
				{
					// Block
					if( line.at( x ) == '#' )
					{
						ColoredRectangle * block = new ColoredRectangle( SNAKE_WIDTH, SNAKE_WIDTH, grey );
						block->getOrigin().moveTo( GAME_AREA_X + x * SNAKE_WIDTH, GAME_AREA_Y + y * SNAKE_WIDTH, 0.0f );
						this->blocks.push_back( block );
					}
					
					// Snake Origin
					else if( line.at( x ) == 'X' )
					{
						this->snakeOrigin.moveTo( x, y );
					}
				}
				
				y++;
			}
			
			#ifdef DEBUG1
			Logger::get() << "[Map] Loaded (" << this->blocks.size() << " blocks)." << Logger::endl;
			Logger::get() << "[Map] Snake origin is (" << this->snakeOrigin.getX() << "," << this->snakeOrigin.getY() << ")." << Logger::endl;
			#endif
			
			file.close();
		}
		#ifdef DEBUG1
		else
			Logger::get() << "[Map] Unable to open the map file \"" << MAPS_DIRECTORY << "/" << title << "\"." << Logger::endl;
		#endif
	}
	
	Map::~Map()
	{
		for( vector<ColoredRectangle *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
			delete (*it);
			
		this->blocks.clear();
	}
	
	const Point2D& Map::getSnakeOrigin() const
	{
		return this->snakeOrigin;
	}
	
	const vector<ColoredRectangle *>& Map::getMap() const
	{
		return this->blocks;
	}
	
	const string& Map::getTitle() const
	{
		return this->title;
	}
	
	bool Map::isInCollision( const ColoredRectangle& rectangle ) const
	{
		bool collision = false;
		
		for( vector<ColoredRectangle *>::const_iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
		{
			if( rectangle.isInCollision( **it ) )
			{
				collision = true;
				break;
			}
		}
		
		return collision;
	}

	void Map::prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices ) const
	{
		if( this->blocks.size() > 0 )
		{
			for( vector<ColoredRectangle *>::const_iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
				(*it)->prepareRendering( vertices, colors, indices );
		}
	}
	
	vector<string> Map::getAvailableMaps()
	{
		vector<string> maps;
		
		DIR * directory = NULL;
		struct dirent * entry = NULL;
		
		directory = opendir( MAPS_DIRECTORY );
		
		if( directory != NULL )
		{
			while( (entry = readdir( directory )) != NULL )
			{
				if( entry->d_type == DT_REG )
				{
					#ifdef DEBUG1
					Logger::get() << "[Maps] Found map \"" << entry->d_name << "\"." << Logger::endl;
					#endif
					
					maps.push_back( string(entry->d_name) );
				}
			}
			
			closedir( directory );
			
			std::sort( maps.begin(), maps.end() );
		}
		else
		{
			#ifdef DEBUG1
			Logger::get() << "[Maps] Could not list maps' directory (\"" << MAPS_DIRECTORY << "\")." << Logger::endl;
			#endif
			
			maps.push_back( "empty" );
		}
		
		return maps;
	}
}

