#include <blockgame/Grid.h>
#include <tools/logger/Logger.h>

#include <map>

using namespace opengl;
using namespace tools::logger;
using namespace std;

namespace blockgame
{
	Grid::Grid( float width, float height ) : Blocks( width * height ), width(width), height(height)
	{
	}
	
	Grid::~Grid()
	{
	}
	
	float Grid::getWidth()
	{
		return this->width;
	}
	
	float Grid::getHeight()
	{
		return this->height;
	}
		
	unsigned int Grid::deleteFullLines()
	{
		unsigned int deletedLines = 0;
		map<float, unsigned int> lines;
		
		for( vector<Block *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
		{
			float y = (*it)->getPosition().getY();
			map<float, unsigned int>::iterator itLines = lines.find( y );
			
			if( itLines == lines.end() )
				lines[ y ] = 1;
			else
				lines[ y ]++;
		}
		
		for( map<float, unsigned int>::iterator it = lines.begin() ; it != lines.end() ; it++ )
		{
			if( it->second == this->width )
			{
				for( vector<Block *>::iterator itBlock = this->blocks.begin() ; itBlock != this->blocks.end() ; )
				{
					if( (*itBlock)->getPosition().getY() == it->first )
					{
						delete *itBlock;
						itBlock = this->blocks.erase( itBlock );
					}
					else
					{
						if( (*itBlock)->getPosition().getY() < it->first )
							(*itBlock)->getPosition().moveBy( 0.0f, 1.0f );

						itBlock++;
					}
				}
				
				deletedLines++;
			}
		}
		
		return deletedLines;
	}
}

