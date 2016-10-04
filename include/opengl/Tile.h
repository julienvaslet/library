#ifndef __OPENGL_TILE_H
#define __OPENGL_TILE_H	1

#include <opengl/Texture2D.h>
#include <opengl/Rectangle.h>

#include <vector>
using namespace std;

namespace opengl
{
	class Tile
	{
		protected:
			Texture2D * texture;
			Rectangle view;
		
		public:
			Tile();
			Tile( const Tile& tile );
			Tile( const string& textureResource );
			Tile( Texture2D * texture );
			virtual ~Tile();
			
			void setView( unsigned int x, unsigned int y, unsigned int width, unsigned int height );
			const Rectangle& getView() const;
			Texture2D * getTexture();
			void setTexture( Texture2D * texture );
			void setTexture( const string& textureResource );
			
			void prepareRendering( vector<Point2D>& textureCoordinates );
	};
}

#endif

