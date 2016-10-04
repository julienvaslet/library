#include <blockgame/Blocks.h>
#include <tools/logger/Logger.h>

using namespace opengl;
using namespace tools::logger;
using namespace std;

namespace blockgame
{
	Blocks::Blocks( unsigned int reserveBlocks )
	{
		if( reserveBlocks > 0 )
			this->blocks.reserve( reserveBlocks );
	}
	
	Blocks::~Blocks()
	{
		for( vector<Block *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
			delete (*it);
	}
	
	Point3D& Blocks::getOrigin()
	{
		return this->origin;
	}
	
	void Blocks::insert( Block * block )
	{
		this->blocks.push_back( block );
	}
	
	void Blocks::insert( Blocks * blocks )
	{
		for( vector<Block *>::iterator it = blocks->blocks.begin() ; it != blocks->blocks.end() ; it++ )
			this->insert( *it );
		
		blocks->blocks.clear();
	}
	
	bool Blocks::isInCollision( Blocks * blocks )
	{
		bool hasCollision = false;
		
		if( this->blocks.size() > 0 )
		{
			for( vector<Block *>::iterator it1 = this->blocks.begin() ; it1 != this->blocks.end() ; it1++ )
			{
				for( vector<Block *>::iterator it2 = blocks->blocks.begin() ; it2 != blocks->blocks.end() ; it2++ )
				{
					if( (*it1)->getPosition().getX() == (*it2)->getPosition().getX() && (*it1)->getPosition().getY() == (*it2)->getPosition().getY() )
					{
						hasCollision = true;
						break;
					}
				}
				
				if( hasCollision )
					break;
			}
		}
		
		return hasCollision;
	}
	
	void Blocks::prepareRendering( vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices )
	{
		for( vector<Block *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
			(*it)->prepareRendering( this->origin, vPoints, vTexCoords, vColors, vIndices );
	}
}

